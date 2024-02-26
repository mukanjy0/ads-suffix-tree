//
// Created by iansg on 2/26/2024.
//

#ifndef ADS_SUFFIX_TREE_SUFFIXTREE_H
#define ADS_SUFFIX_TREE_SUFFIXTREE_H

#include <map>
#include <string>
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
        m = (int)s.size();
        s.append("$");
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
        Node* u = root;
        for (int i = 0; i < m; ++i) {
            Node* prev {};

            for (; j <= i+1; ++j) {
                if (pos != u->r) {
                    k -= u->length();
                    u = u->par;
                }
                u = u->link;

                int len = i+1-j;
                while (k < len) {
                    u = u->children[s[j+k]];
                    k += u->length();
                }
                pos = u->r - (k-len);
                if (pos==u->r) {
                    if (u->children.count(s[i+1])) break;
                    if (u->children.empty()) {
                        int dif = u->r - u->l;
                        u->l = i+2-dif;
                        u->r = i+2;
                    }
                    else {
                        v = new Node(i+1,m,u,i+1);
                        u->children[s[j]] = v;
                        if (prev) {
                            prev->link = u;
                            prev = nullptr;
                        }
                    }
                }
                else {
                    if (s[u->l + pos] == s[i+1]) break;
                    k -= u->r - u->l;

                    Node* p = new Node(u->l, u->l+pos, u->par);
                    u->par->children[s[u->l]] = p;
                    p->children[s[u->l + pos]] = u;
                    u->par = p;
                    u->l = u->l + pos;
                    v = new Node(i+2 - (u->r - pos),m,p,i+1);
                    p->children[s[i+1]] = v;

                    if (prev) prev->link = p;
                    prev = p;

                    u = p->par;
                    pos = p->par->r;
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
            for (int j = u->l; j < u->r; ++j,++i) {
                if (i==n) return true;
                cout << j << ' ' << i << '\n';
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

#endif //ADS_SUFFIX_TREE_SUFFIXTREE_H
