#include <iostream>

int main() {

    char c {'a'};
    while (c != 'z')
    {
        std::cout << "Character: " << c << ", ASCII code: " << static_cast<int>(c) << '\n';
        ++c;
    }

    std::cout << "Character: " << c << ", ASCII code: " << static_cast<int>(c) << '\n';

    return 0;
}