//error4.cpp
#include <iostream>
using namespace std;
#include <cmath> // or math.h, unix users may need -lm flag
double hmean(double a, double b) throw(const char *);
double gmean(double a, double b) throw(const char *);

int main()
{
    double x, y, z;
    cout << "Enter two numbers: ";
    try {                // start of try block
        while (cin >> x >> y)
        {
            z = hmean(x,y);
            cout << "Harmonic mean of " << x << " and " << y
                << " is " << z << "\n";
            cout << "Geometric mean of " << x << " and " << y
                << " is " << gmean(x,y) << "\n";
            cout << "Enter next set of numbers <q to quit>: ";
        }
    }                   // end of try block
    catch (const char * s)    // start of catch block
    {
        cout << s << "\n";
        cout << "Sorry, you don't get to play any more. ";
    }                   // end of catch block
    cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b) throw(const char *)
{
    if (a == -b)
        throw "bad hmean() arguments: a = -b not allowed";
    return 2.0 * a * b / (a + b);
}

double gmean(double a, double b) throw(const char *)
{
    if (a < 0 || b < 0)
        throw "bad gmean() arguments: negative values not allowed";
    return sqrt(a * b);
}

