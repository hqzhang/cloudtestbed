// usestok1.cpp -- use the Stock class
#include <iostream>
using namespace std;
#include "stock1.h"

int main()
{
// using constructors to create new objects
    Stock stock1("NanoSmart", 12, 20.0);            // syntax 1
    Stock stock2 = Stock ("Boffo Objects", 2, 2.0); // syntax 2

    cout.precision(2);                          // #.## format
    cout.setf(ios::fixed, ios::floatfield);     // #.## format
    cout.setf(ios::showpoint);                  // #.## format

    stock1.show();
    stock2.show();
    stock2 = stock1;                        // object assignment

// using a constructor to reset an object
    stock1 = Stock("Nifty Foods", 10, 50.0);    // temp object

    cout << "After stock reshuffle:\n";
    stock1.show();
    stock2.show();
    return 0;
}

