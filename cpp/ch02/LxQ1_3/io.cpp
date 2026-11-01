#include "io.h" // don't forget corresponding header for best practice!
#include <iostream>

// Function that reads the number entered by the user
int readNumber()
{
    std::cout << "Enter integer number here: ";

    int x{};
    std::cin >> x;
    
    return x;
}

// Function that prints out the integer number provided
void writeAnswer(int x)
{
    std::cout << "The sum is " << x << ".\n";
}