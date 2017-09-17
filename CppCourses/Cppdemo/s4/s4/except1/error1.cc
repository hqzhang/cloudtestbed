//error1.cpp -- use the abort() function
#include <iostream>
using namespace std;
#include <cstdlib>
double hmean(double a, double b);

int main()
{
    double x, y, z;

    cout << "Enter two numbers: ";
    while (cin >> x >> y)
    {
        z = hmean(x,y);
        cout << "Harmonic mean of " << x << " and " << y
            << " is " << z << "\n";
        cout << "Enter next set of numbers <q to quit>: ";
    }
    cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if (a == -b)
    {
        cout << "untenable arguments to hmean()\n";
        abort();
    }
    return 2.0 * a * b / (a + b);
}

