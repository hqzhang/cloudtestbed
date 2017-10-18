// strng2.h -- String class definition
#ifndef _STRNG2_H_
#define _STRNG2_H_
#include <iostream>
using namespace std;
class String
{
private:
    char * str;                   // pointer to string
    int len;                      // length of string
public:
    String(const char * s);       // constructor
    String();                     // default constructor
    String(const String & st);
    ~String();                    // destructor
    int length() const { return len; }
// overloaded operators
    String & operator=(const String & st); // Assignment operator
    String & operator=(const char * s);    // Assignment operator #2
// friend functions
    friend bool operator>(const String &st1, const String &st2);
    friend bool operator<(const String &st, const String &st2);
    friend bool operator==(const String &st, const String &st2);
    friend ostream & operator<<(ostream & os, const String & st);
    friend istream & operator>>(istream & is, String & st);
};
#endif

