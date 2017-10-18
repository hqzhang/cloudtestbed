// problem3.cpp -- assigns one object to another
// compile with strng1.cpp
#include <iostream>
using namespace std;
#include "strng1.h"

int main()
{
    String1 motto("Home Sweet Home");
    String1 ditto;        // default constructor
    ditto = motto;        // object assignment
    cout << motto << "\n";
    cout << ditto << "\n";
    return 0;
}

