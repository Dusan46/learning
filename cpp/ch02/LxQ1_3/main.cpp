#include "io.h"

int main()
{

    // Take two numbers from the user
    int a{readNumber()};
    int b{readNumber()};
    
    // Print their sum
    writeAnswer(a+b);

    return 0;
}