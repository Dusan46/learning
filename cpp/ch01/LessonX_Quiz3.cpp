#include <iostream>
// Code that solves Quiz #3 of lesson 1.X from
// https://www.learncpp.com/cpp-tutorial/chapter-1-summary-and-quiz/
/*
It asks user to enter two integer numbers 
and outputs their sum and difference.
*/



int main()
{
    std::cout << "Enter an integer: ";

    int x{};
    std::cin >> x;

    std::cout << "Enter another integer: ";

    int y{};
    std::cin >> y;

    std::cout << x << " + " << y << " is " << x+y << ".\n";
    std::cout << x << " - " << y << " is " << x-y << ".\n";

    return 0;
}