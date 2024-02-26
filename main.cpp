#include <iostream>
#include <string>
#include "SuffixTree.h"
using namespace std;

void test() {
    string s = "awyawxawxz";
    suffix_tree sf (s);
    cout << boolalpha << sf.search("aw") << '\n';
    cout << boolalpha << sf.search("ya") << '\n';
    cout << boolalpha << sf.search("xz") << '\n';
    cout << boolalpha << sf.search("zx") << '\n';
}

int main() {
    test();
    return 0;
}
