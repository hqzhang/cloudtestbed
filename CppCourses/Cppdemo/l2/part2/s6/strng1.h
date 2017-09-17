// strng1.h -- string class definition
#include <iostream>
using namespace std;
#ifndef _STRNG1_H_
#define _STRNG1_H_
class String1
{
private:
    char * str;             // pointer to string
    int len;                // length of string
    static int num_strings; // number of objects
public:
    String1(const char * s); // constructor
    String1();               // default constructor
    ~String1();              // destructor
// friend function
friend ostream & operator<<(ostream & os, const String1 & st);
friend void show(const String1 &st);
};
#endif

