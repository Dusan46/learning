#include <iostream>
#include <string_view>

std::string_view getQuantityPhrase(int x)
{
    if ( x<0 )
        return "negative";
    else if ( x == 0)
        return "no";
    else if (x == 1)
        return "a single";
    else if ( x == 2)
        return "a couple of";
    else if ( x == 3)
        return "a few";
    else
        return "many";

    
}

std::string_view getApplesPluralized(int x)
{
    return (x == 1) ? "apple" : "apples";
}

int main()
{
    constexpr int maryApples { 3 };
    std::cout << "Mary has " << getQuantityPhrase(maryApples) << ' ' << getApplesPluralized(maryApples) << ".\n";

    std::cout << "How many apples do you have? ";
    int numApples{};
    std::cin >> numApples;

    std::cout << "You have " << getQuantityPhrase(numApples) << ' ' << getApplesPluralized(numApples) << ".\n";

    return 0;
}