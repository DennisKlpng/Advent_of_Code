#include "utils.hpp"

int main() {
    std::vector<int> a{0, 3};
    int b = 3;
    std::string c = "abc";
    std::map<int, std::string> d{{0, "first entry"}, {1, "second entry"}};
    print(b, c);
    print(a);
    print(d);

    std::vector<int> vals{0, 5, 3, 4, 8, 7};
    print(std::ranges::max(vals));
    print("END");    
    return 0;
}