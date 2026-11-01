#include <iostream>

double distanceFallen(double sec)
{
    return 0.5 * 9.8 * sec * sec;
}

void printHeight(double h, double x)
{
    if (distanceFallen(x) < h)
         std::cout << "At " << x << " seconds, the ball is at height: " <<
            h - distanceFallen(x) << " meters \n";
    else 
        std::cout << "At " << x << " seconds, the ball is on the ground. \n";
}

int main()
{
    std::cout << "Enter the height of the tower in meters: ";
    double h{};
    std::cin >> h;

    printHeight(h,0.0);
    printHeight(h,1.0);
    printHeight(h,2.0);
    printHeight(h,3.0);
    printHeight(h,4.0);
    printHeight(h,5.0);
    

    return 0;
}