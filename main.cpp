#include <iostream>
#include <string>
#include "SuffixTree.h"
using namespace std;

void test() {
    string s = "awyawxawxz";
    suffix_tree sf (s);
    cout << boolalpha << sf.search("aw") << '\n';
    cout << boolalpha << sf.search("wy") << '\n';
    cout << boolalpha << sf.search("wxz") << '\n';
    cout << boolalpha << sf.search("x") << '\n';
    cout << boolalpha << sf.search("z") << '\n';
    cout << boolalpha << sf.search(s) << '\n';
}

int main() {
    test();
    return 0;
}
