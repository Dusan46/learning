#include <iostream>

double getNumber()
{
    std::cout << "Enter a double value: ";
    double x{};
    std::cin >> x;

    return x;
}

char getSign()
{

    std::cout << "Enter +, -, * or /: ";
    char c{};
    std::cin >> c;

    return c;
}

int main()
{


    double x{getNumber()};
    double y{getNumber()};

    char sign{getSign()};

    // if (sign == '+' || sign == '-' || sign == '*' || sign == '/')
    //     std::cout << x << sign << y << " is " << x*sign*y << '\n';
    if (sign == 43)
        std::cout << x << sign << y << " is " << x+y << '\n';
    else if (sign == 45)
        std::cout << x << sign << y << " is " << x-y << '\n';
    else if (sign == 42)
        std::cout << x << sign << y << " is " << x*y << '\n';
    else if (sign == 47)
        std::cout << x << sign << y << " is " << x/y << '\n';
    
    return 0;
}