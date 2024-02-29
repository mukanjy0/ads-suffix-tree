#include <bits/stdc++.h>
using namespace std;

// get TLE verdict because all tests cases (>900) are run together
// if user selects "use test case" and "Run", no TLE
// in conclusion, TLE because of constant factor, not complexity
// to get AC, must use algorithm with small constant factor such as KMP or Z
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

    // returns the index of the longest suffix of S (starting at index multiple of k) that is prefix of S
    int longest_suffix_that_is_prefix(int k) {
        if (root == nullptr) return (m-2+k)/k;
        Node* u = root;
        int i = 0, j = m-2+k;
        while (i<m) {
            if (u->children.count(0))
                if (u->children[0]->j%k==0)
                    j = u->children[0]->j;
            u = u->children[s[i]];
            i+=u->length();
        }
        return j/k;
    }

    void clear() {
         _clear(root);
        root = nullptr;
    }
};

class Solution {
public:
    int minimumTimeToInitialState(string word, int k) {
        suffix_tree sf (word);
        return sf.longest_suffix_that_is_prefix(k);
    }
};

void test() {
    string s = "abacaba";
    cout << Solution().minimumTimeToInitialState(s,3) << '\n';
}

int main() {
    test();
    return 0;
}