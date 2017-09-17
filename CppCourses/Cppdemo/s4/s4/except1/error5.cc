//error5.cpp
#include <iostream>
using namespace std;
double hmean(double a, double b) throw(const char *);
void details(double a, double b) throw(const char *);
int main()
{
    double x, y;

    cout << "Enter two numbers: ";
    try {
        while (cin >> x >> y)
            details(x,y);
    }
    catch (const char * s)
    {
        cout << s << "\n";
        cout << "Sorry, you can't play anymore. ";
    }
    cout << "Bye!\n";
    return 0;
}

void details(double a, double b) throw(const char *){
    cout << "Harmonic mean of " << a << " and " << b
        << " is " << hmean(a,b) << "\n";
    cout << "Enter next set of numbers <q to quit>: ";
}

double hmean(double a, double b) throw(const char *)
{
    if (a == -b)
        throw "bad hmean() arguments: a = -b not allowed";
    return 2.0 * a * b / (a + b);
}


