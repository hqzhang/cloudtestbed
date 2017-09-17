//error3.cpp
#include <iostream>
using namespace std;
double hmean(double a, double b);

int main()
{
    double x, y, z;

    cout << "Enter two numbers: ";
    while (cin >> x >> y)
    {
        try {                   // start of try block
            z = hmean(x,y);
        }                       // end of try block
        catch (const char * s)  // start of exception handler
        {
            cout << s << "\n";
            cout << "Enter a new pair of numbers: ";
            continue;
        }                       // end of handler
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
        throw "bad hmean() arguments: a = -b not allowed";
    return 2.0 * a * b / (a + b);
}

