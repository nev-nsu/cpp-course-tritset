#include <iostream>
#include "Tritset.h"

int main() {
    Tritset a(10, Trit(true));
    std::string s = a.to_string();
    std::cout << sizeof(trit) << std::endl << s << std::endl;
    return 0;
}