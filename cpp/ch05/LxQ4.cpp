#include <iostream>
#include <string>
#include <string_view>

int main()
{
    std::cout << "Enter the name of person #1: ";
    std::string name1 {};
    std::getline(std::cin >> std::ws, name1);
    std::string_view vname1 {name1};

    std::cout << "Enter the age of " <<  vname1 << ": ";
    int age1 {};
    std::cin >> age1;

    std::cout << "Enter the name of person #2: ";
    std::string name2 {};
    std::getline(std::cin >> std::ws, name2);
    std::string_view vname2 {name2};

    std::cout << "Enter the age of " <<  vname2 << ": ";
    int age2 {};
    std::cin >> age2;

    if (age1 > age2)
        std::cout << vname1 << " (age " << age1 << ") is older than " <<
     vname2 << " (age " << age2 << ").\n";
    else if (age1 == age2)
        std::cout << vname1 << " is of same age as " << vname2;
    else
        std::cout << vname2 << " (age " << age2 << ") is older than " <<
     vname1 << " (age " << age1 << ").\n";
    return 0;
}