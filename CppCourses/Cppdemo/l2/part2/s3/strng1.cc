// strng1.cpp -- String class methods
#include <iostream>
#include <cstring>
#include "strng1.h"
using namespace std;

// initializing static class member
int String::num_strings = 0;

// class methods
String::String(const char * s)     // construct String from C string
{
    len = strlen(s);               // set size
    str = new char[len + 1];       // allot storage
    strcpy(str, s);                // initialize pointer
    num_strings++;                 // set object count
    cout << num_strings << ": \"" << str
         << "\" object created\n"; // For Your Information
}

String::String()                   // default constructor
{
    len = 4;
    str = new char[4];
    strcpy(str, "C++");            // default string
    num_strings++;
    cout << num_strings << ": \"" << str
         << "\" default object created\n";  // FYI
}

String::~String()                  // necessary destructor
{
    cout << "\"" << str << "\" object deleted, ";    // FYI
    --num_strings;                 // required
    cout << num_strings << " left\n"; // FYI
    delete [] str;                 // required
}

ostream & operator<<(ostream & os, const String & st)
{
    os << st.str;
    return os;
}

