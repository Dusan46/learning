#include "Random.h" // defines Random::mt, Random::get(), and Random::generate()
#include <cstddef> // for std::size_t
#include <iostream>

bool guess(int cor, int i){
    std::cout << "Gusess #" << i << ": ";
    int userNum {};
    std::cin >> userNum;

    if (userNum > cor)
    {
        std::cout << "Your guess is too high\n";
    }
    else if (userNum < cor)
    {
        std::cout << "Your guess is too low\n";
    }
    else{
        std::cout << "Congrats! You won.\n";
        return true;
    }
    
    return false;
}

int main()
{
    std::cout << "Let's play a game. I'm thinking of a number between 1 and 100. You have 7 tries to guess what it is.\n";
	
    while (true)
    {
    const int correct { Random::get(1, 100) };
    bool ctn {false};

    for (int i {1}; i <= 7; ++i)
    {
        ctn = guess(correct, i);
        if (ctn)
            break;
    }

    if (!ctn)
        std::cout << "Sorry, you lost :(\n";

    AskAgain:
    std::cout << "Would you like to play again (y/n)? ";
    char c {};
    std::cin >> c;
    if (c == 'n')
        break;
    if (c != 'y')
        goto AskAgain;
    }

    std::cout << "Thank you for playing.\n";
	return 0;
}