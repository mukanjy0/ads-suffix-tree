//
// Created by iansg on 2/26/2024.
//

#include <bits/stdc++.h>
using namespace std;

class suffix_tree {
    class Node {
        int l {}; // inclusive left bound
        int r {}; // exclusive right bound
        int j {};
        Node* par {};
        Node* link {};
        map<char,Node*> children;
        friend suffix_tree;
    public:
        Node() : j(-1), link(this) {}
        Node(int l, int r, Node* par = nullptr, int j = -1) : l(l), r(r), par(par), j(j) {}
        [[nodiscard]] int length() const { return r-l; }
    };

    void _clear(Node* u) {
        for (auto [c, v] : u->children) _clear(v);
        delete u;
    }

    string s;
    int m;
    Node* root {};
public:
    explicit suffix_tree(string s) : s(s) {
        s.append("$");
        m = (int)s.size();
        build();
    }

    void build() {
        if (s.empty()) return;

        root = new Node(1, 1);
        root->link = root;

        Node* v = new Node(0,m,root,0);
        root->children[s[0]] = v; // I_1

        int j = 1; // j* explicit extension start
        int k = 0;
        int pos = 1;
        bool implicit = true;
        Node* u = root;
        for (int i = 0; i < m; ++i) {
            Node* prev {};

            for (; j <= i+1; ++j) {
                if (!implicit) {
                    if (pos != u->r) {
                        k -= u->length();
                        u = u->par;
                    }
                    u = u->link;
                    k = max(0,k-1);
                }
                else implicit = false;

                int len = i+1-j;
                while (k < len) {
                    u = u->children[s[j+k]];
                    k += u->length();
                }
                pos = u->r - (k-len);
                if (pos==u->r) {
                    if (u->children.count(s[i+1])) { implicit = true; break; }
                    if (u->children.empty()) {
                        int dif = u->r - u->l;
                        u->l = i+2-dif-1; //change
                    }
                    else {
                        v = new Node(i+1,m,u,j);
                        u->children[s[j]] = v;
                        if (prev) {
                            prev->link = u;
                            prev = nullptr;
                        }
                    }
                }
                else {
                    if (s[pos] == s[i+1]) { implicit = true; break; }
                    k -= u->length();

                    Node* p = new Node(u->l, pos, u->par);
                    u->par->children[s[u->l]] = p;
                    p->children[s[pos]] = u;
                    u->par = p;
                    u->l = pos;
                    v = new Node(i+1,m,p,j);
                    p->children[s[i+1]] = v;

                    if (prev) prev->link = p;
                    prev = p;

                    u = p->par;
                    pos = u->r;
                }
            }
        }
    }

    bool search(const string& p) {
        if (root == nullptr) return false;
        Node* u = root;
        int i = 0, n = (int)p.size();
        while (i < n) {
            if (!u->children.count(p[i])) return false;
            u = u->children[p[i]];
            cout << u->l << ' ' << u->r << '\n';
            for (int j = u->l; j < u->r; ++j,++i) {
                if (i==n) return true;
                if (s[j] != p[i]) return false;
            }
        }
        return true;
    }

    void clear() {
        _clear(root);
        root = nullptr;
    }
};

int slv() {
    string s; cin >> s;
    suffix_tree sf (s);
    int q; cin >> q;
    while (q--) {
        string t; cin >> t;
        cout << (sf.search(t) ? 'y' : 'n') << '\n';
    }
    return 0;
}

int main() {
    int k;
    cin >> k;
    while (k--) slv();
    return 0;
}
