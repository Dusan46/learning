#include <iostream>

int sumTo(int value){

    int sum {};
    for (int i {1}; i <= value; ++i)
        sum += i;

    return sum;
}

int main() {

    std::cout << "Enter an integer: ";
    int value {};
    std::cin >> value;

    std::cout << "The sum is " << sumTo(value) << '\n';

    return 0;
}