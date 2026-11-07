#include <iostream>

int main()
{
    std::cout << "Enter an integer: ";
    int smaller {};
    std::cin >> smaller;

    std::cout << "Enter a larger integer: ";
    int larger {};
    std::cin >> larger;

    if ( larger < smaller)
    { // start a block - smaller and larger are still visible inside
        std::cout << "Swaping the variables \n";

        int tmp {smaller}; // temporary variable only visible inside the block

        smaller = larger;
        larger = tmp;
    }// tmp is destroyed
    
    std::cout << "The smaller value is " << smaller << '\n';
    std::cout << "The larger value is " << larger << '\n';


    return 0;
} // smaller and larger are destroyed