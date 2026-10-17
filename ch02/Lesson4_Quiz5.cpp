#include <iostream>

int doubleNumber(int x)
{
    return 2*x;
}

int main()
{
    std::cout << "Enter an integer #: ";
    
    int num{};
    std::cin >> num;

    std::cout << "Doubled value: " << doubleNumber(num) << ".\n";


    return 0;
}