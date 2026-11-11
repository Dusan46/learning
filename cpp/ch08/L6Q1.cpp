#include <iostream>

int calculate(int a, int b, char c){

    switch (c)
    {
    case '+':
        return a+b;
    case '-':
        return a-b;
    case '*':
        return a*b;
    case '/':
        return a/b;
    case '%':
        return a%b;
    default:
        std::cout << "error! invalid operator!";
        return 0;
    }

    
}

int main() {
    std::cout << "Enter first integer: ";
    int x {};
    std::cin >> x;

    std::cout << "Enter second integer: ";
    int y {};
    std::cin >> y;

    std::cout << "Enter operation sign: ";
    char c {};
    std::cin >> c;

    std::cout << "The result is " << calculate(x,y,c) << '\n';

    return 0;
}