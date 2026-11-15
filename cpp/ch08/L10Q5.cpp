#include <iostream>

void fizzbuzz(int value){

    for (int i {1}; i <= value; ++i)
        if ( (i%3) && (i%5) && (i%7))
            std::cout << i << '\n';
        else if ( ~(i%3) && (i%5) && (i%7))
            std::cout <<  "fizz" << '\n';
        else if ( (i%3) && ~(i%5) && (i%7))
            std::cout <<  "buzz" << '\n';
        else if ( (i%3) && (i%5) && ~(i%7))
            std::cout <<  "pop" << '\n';
        else 
            std::cout <<  "fizzbuzz" << '\n';
        
}

int main() {

    std::cout << "Enter an integer: ";
    int value {};
    std::cin >> value;

    fizzbuzz(value);

    return 0;
}