// arraydbe.h -- define array class with exceptions
#ifndef _ARRAYDBE_H_
#define _ARRAYDBE_H_
#include <iostream>
using namespace std;

class ArrayDbE
{
private:
    unsigned int size;  // number of array elements
protected:
    double * arr;       // address of first element
public:
    class BadIndex      // excepton class for indexing problems
    {
    public:
        int badindex;   // problematic index value
        BadIndex(int i) : badindex(i) {}
    };
    ArrayDbE();                          // default constructor
    // create an ArrayDbE of n elements, set each to val
    ArrayDbE(unsigned int n, double val = 0.0);
    // create an ArrayDbE of n elements, initialize to array pn
    ArrayDbE(const double * pn, unsigned int n);
    // copy constructor
    ArrayDbE(const ArrayDbE & a);
    virtual ~ArrayDbE();                 // destructor
    unsigned int ArSize() const;        // returns array size
    double Average() const;             // return array average
// overloaded operators
        // array indexing, allowing assignment
    virtual double & operator[](int i) throw(BadIndex &);
        // array indexing (no =)
    virtual const double & operator[](int i) const throw(BadIndex &);
    ArrayDbE & operator=(const ArrayDbE & a);
    friend ostream & operator<<(ostream & os, const ArrayDbE & a);
};

#endif
