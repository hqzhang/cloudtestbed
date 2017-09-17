// limarre.cpp
#include "limarre.h"
#include <iostream>
using namespace std;

// private method
   // lower bound for array index is now low_bnd, and 
   // upper bound is now low_bnd + size - 1
void LimitArE::ok(int i) const throw(LimitArE::SonOfBad &)
{
    unsigned long size = ArSize();
    if (i < low_bnd || i >= size + low_bnd)
        throw SonOfBad(i, low_bnd, low_bnd + size - 1);
}

// redefined operators
double & LimitArE::operator[](int i)
{
   ok(i);
   return arr[i - low_bnd];
}

const double & LimitArE::operator[](int i) const
{
   ok(i);
   return arr[i - low_bnd];
}

