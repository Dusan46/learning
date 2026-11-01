#include <iostream>

int main()
{
    std::cout << "Enter a single character: ";
    char a {};
    std::cin >> a;

    std::cout << "You enetered \'" << a << "\', which has ASCII code " << static_cast<int>(a) << ".\n";


    return 0;
}