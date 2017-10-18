// problem2.cpp -- initializes one string to another
// compile with strng1.cpp
#include <iostream>
using namespace std;
#include "strng1.h"

int main()
{
    String1 motto("Home Sweet Home");
    String1 ditto(motto);    // initialize ditto to motto

    cout << motto << "\n";
    cout << ditto << "\n";
    return 0;
}

