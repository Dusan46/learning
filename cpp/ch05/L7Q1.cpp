#include <iostream>
#include <string>

int main()
{
    std::cout << "Enter your full name: ";
    std::string name {};
    std::getline(std::cin >> std::ws, name);

    std::cout << "Enter your age: ";
    int age {};
    std::cin >> age;

    int name_length {static_cast<int>(name.length())};
    std::cout << "Your age + length of name is: " << name_length + age << '\n';

    return 0;
}