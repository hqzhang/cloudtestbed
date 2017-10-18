// problem1.cpp -- uses a function with a String argument
// compile with strng1.cpp
#include <iostream>
using namespace std;
#include "strng1.h"

void showit(String1 s, int n);
int main()
{
    String1 motto("Home Sweet Home");
    showit(motto, 3);
    return 0;
}

void showit(String1 s, int n)    // show String s n times
{
    for (int i = 0; i < n; i++)
        cout << s << "\n";
}

