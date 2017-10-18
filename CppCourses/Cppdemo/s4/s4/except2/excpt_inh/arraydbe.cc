// arraydbe.cpp -- ArrayDbE class methods
#include <iostream>
using namespace std;
#include "arraydbe.h"

// default constructor -- no arguments
ArrayDbE::ArrayDbE()
{
   arr = NULL;
   size = 0;
}

// constructs array of n elements, each set to val
ArrayDbE::ArrayDbE(unsigned int n, double val)
{
    arr = new double[n];
    size = n;
    for (int i = 0; i < size; i++)
       arr[i] = val;
}

// initialize ArrayDbE object to a non-class array
ArrayDbE::ArrayDbE(const double *pn, unsigned int n)
{
    arr = new double[n];
    size = n;
    for (int i = 0; i < size; i++)
       arr[i] = pn[i];
}

// initialize ArrayDbE object to another ArrayDbE object
ArrayDbE::ArrayDbE(const ArrayDbE & a)
{
    size = a.size;
    arr = new double[size];
    for (int i = 0; i < size; i++)
       arr[i] = a.arr[i];
}

ArrayDbE::~ArrayDbE()
{
   delete [] arr;
}

double ArrayDbE::Average() const
{
    double sum = 0;
    int i;
    int lim = ArSize();
    for (i = 0; i < lim; i++)
        sum += arr[i];
    if (i > 0)
        return sum / i;
    else
    {
        cerr << "No entries in score array\n";
        return 0;
    }
}

// return array size
unsigned int ArrayDbE::ArSize() const
{
   return size;
}

// let user access elements by index (assignment allowed)
double & ArrayDbE::operator[](int i) throw(BadIndex &)
{
    // check index before continuing
    if (i < 0 || i >= size)
        throw BadIndex(i);
    return arr[i];
}

// let user access elements by index (assignment disallowed)
const double & ArrayDbE::operator[](int i)const throw(ArrayDbE::BadIndex &) 
{
    // check index before continuing
    if (i < 0 || i >= size)
        throw BadIndex(i);
    return arr[i];
}

// define class assignment
ArrayDbE & ArrayDbE::operator=(const ArrayDbE & a) 
{
    if (this == &a)      // if object assigned to self,
        return *this;    // don't change anything
    delete arr;
    size = a.size;
    arr = new double[size];
    for (int i = 0; i < size; i++)
       arr[i] = a.arr[i];
    return *this;
}

// quick output, 5 values to a line
ostream & operator<<(ostream & os, const ArrayDbE & a)
{
   int i;
   for (i = 0; i < a.size; i++)
   {
       os << a.arr[i] << " ";
       if (i % 5 == 4)
           os << "\n";
   }
   if (i % 5 != 0)
       os << "\n";
   return os;
}

