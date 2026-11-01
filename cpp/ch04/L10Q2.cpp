#include <iostream>

int main()
{

    int x{};
    std::cout << "Enter an integer from 0 to 9: ";
    std::cin >> x;
    
    if (x == 2 | x == 3 | x == 5 | x == 7)
        std::cout << "The digit is prime \n";
    else
        std::cout << "The digit is not prime \n";

    return 0;
}