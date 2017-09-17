// strng1.h -- string class definition
#include <iostream>
using namespace std;
#ifndef _STRNG1_H_
#define _STRNG1_H_
class String
{
private:
    char * str;             // pointer to string
    int len;                // length of string
    static int num_strings; // number of objects
public:
    String(const char * s); // constructor
    String();               // default constructor
    ~String();              // destructor
// friend function
    friend ostream & operator<<(ostream & os, const String & st);
};
#endif

