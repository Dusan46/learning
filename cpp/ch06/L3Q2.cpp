#include <iostream>

int getInt()
{
    std::cout << "Enter an integer: ";
    int a {};
    std::cin >> a;

    return a;
}

constexpr bool isEven(int x)
{
    return (x % 2) == 0;
}

int main()
{

    int x {getInt()};
    if (isEven(x))
        std::cout << x << " is even.\n";
    else
        std::cout << x << " is odd.\n";

    return 0;
}