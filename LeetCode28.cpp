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
        fill_first_indices();
    }

    ~suffix_tree() { clear(); }

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
        for (int i = 0; i < m-1; ++i) {
            Node* prev {};

            for (; j <= i+1; ++j) {
                if (!implicit) {
                    if (pos != u->r) {
                        k -= u->length();
                        u = u->par;
                    }
                    k = max(0,k-1);
                    u = u->link;
                }
                else implicit = false;

                int len = i+1-j;
                while (k < len) {
                    u = u->children[s[j+k]];
                    k += u->length();
                }
                pos = u->r - (k-len);
                if (pos==u->r) {
                    if (prev) {
                        prev->link = u;
                        prev = nullptr;
                    }

                    if (u->children.count(s[i+1])) { implicit = true; break; }

                    v = new Node(i+1,m,u,j);
                    u->children[s[i+1]] = v;
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

    void fill_first_indices() {
        if (root == nullptr) return;
        stack<Node*> st;
        st.push(root);
        map<Node*,bool> vis;
        while (!st.empty()) {
            auto u = st.top();
            if (u->j >= 0) {
                st.pop();
                continue;
            }
            if (vis.count(u)) {
                st.pop();
                u->j = m;
                for (const auto& [c, v] : u->children)
                    u->j = min(u->j, v->j);
                continue;
            }
            vis[u]=true;
            for (const auto& [c, v] : u->children)
                st.push(v);
        }
    }

    int search(const string& p) {
        if (root == nullptr) return false;
        Node* u = root;
        int i = 0, n = (int)p.size();
        while (i < n) {
            if (!u->children.count(p[i])) return -1;
            u = u->children[p[i]];
            for (int j = u->l; j < u->r; ++j,++i) {
                if (i==n) return u->j;
                if (s[j] != p[i]) return -1;
            }
        }
        return u->j;
    }

    void clear() {
        _clear(root);
        root = nullptr;
    }
};

class Solution {
public:
    int strStr(string haystack, string needle) {
        suffix_tree sf (haystack);
        return sf.search(needle);
    }
};