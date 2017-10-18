// strng1.cpp -- String1 class methods
#include <iostream>
#include <cstring>
#include "strng1.h"
using namespace std;

// initializing static class member
int String1::num_strings = 0;

// class methods
String1::String1(const char * s)     // construct String1 from C String1
{
    len = strlen(s);               // set size
    str = new char[len + 1];       // allot storage
    strcpy(str, s);                // initialize pointer
    num_strings++;                 // set object count
    cout << num_strings << ": \"" << str
         << "\" object created\n"; // For Your Information
}

String1::String1()                   // default constructor
{
    len = 4;
    str = new char[4];
    strcpy(str, "C++");            // default String1
    num_strings++;
    cout << num_strings << ": \"" << str
         << "\" default object created\n";  // FYI
}

String1::~String1()                  // necessary destructor
{
    cout << "\"" << str << "\" object deleted, ";    // FYI
    --num_strings;                 // required
    cout << num_strings << " left\n"; // FYI
    delete [] str;                 // required
}

ostream & operator<<(ostream & os, const String1 & st){
    os << st.str;
    return os;
}

void show(const String1 & st){
	cout<<st.str;
}
