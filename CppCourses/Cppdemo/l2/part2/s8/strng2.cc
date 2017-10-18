// strng2.cpp -- String class methods
#include <iostream>
#include <cstring>
using namespace std;
#include "strng2.h"

// class methods

String::String(const char * s)  // make String from C string
{
    len = strlen(s);
    str = new char[len + 1];    // allot storage
    strcpy(str, s);             // initialize pointer
}

String::String()                // default constructor
{
    len = 0;
    str = new char[1];
    str[0] = '\0';              // default string
}

String::String(const String & st)// copy constructor
{
    len = st.len;
    str = new char[len + 1];
    strcpy(str, st.str);
}

String::~String()                 // destructor
{
    delete [] str;                // required
}

    // assign a String to a String
String & String::operator=(const String & st)
{
    if (this == &st)
        return *this;
    delete [] str;
    len = st.len;
    str = new char[len + 1];
    strcpy(str, st.str);
    return *this;
}

    // assign a C string to a String
String & String::operator=(const char * s)
{
    delete [] str;
    len = strlen(s);
    str = new char[len + 1];
    strcpy(str, s);
    return *this;
}
    // true if st1 follows st2 in collating sequence
bool operator>(const String &st1, const String &st2)
{
    if (strcmp(st1.str, st2.str) > 0)
        return true;
    else
        return false;
}

    // true if st1 precedes st2 in collating sequence
bool operator<(const String &st1, const String &st2)
{
    if (strcmp(st1.str, st2.str) < 0)
        return true;
    else
        return false;
}

// friends
    // true if st1 is the same as st2
bool operator==(const String &st1, const String &st2)
{
    if (strcmp(st1.str, st2.str) == 0)
        return true;
    else
        return false;
}

    // display string
ostream & operator<<(ostream & os, const String & st)
{
    os << st.str;
    return os;
}

    // quick and dirty String input
istream & operator>>(istream & is, String & st)
{
    char temp[80];
    is.get(temp, 80);
    if (is)
        st = temp;
    while (is && is.get() != '\n')
        continue;
    return is;
}

