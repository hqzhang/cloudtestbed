// limarre.h -- LimitArE class with exceptions

#ifndef _LIMARRE_H_
#define _LIMARRE_H_

#include "arraydbe.h"

class LimitArE : public ArrayDbE
{
public:
    class SonOfBad : public ArrayDbE::BadIndex
    {
    public:
        int l_lim;
        int u_lim;
        SonOfBad(int i, int l, int u) : BadIndex(i),
                l_lim(l), u_lim(u) {}
    };
private:
    unsigned int low_bnd;          // new data member
protected:
    // handle bounds checking
    virtual void ok(int i) const throw(SonOfBad &);
public:
// constructors
    LimitArE() : ArrayDbE(), low_bnd(0) {}
    LimitArE(unsigned int n, double val = 0.0)
                : ArrayDbE(n,val), low_bnd(0) {}
    LimitArE(unsigned int n, int lb, double val = 0.0)
                : ArrayDbE(n, val), low_bnd(lb) {}
    LimitArE(const double * pn, unsigned int n)
                : ArrayDbE(pn, n), low_bnd(0) {}
    LimitArE(const ArrayDbE & a) : ArrayDbE(a), low_bnd(0) {}
// new methods
    void new_lb(int lb) {low_bnd = lb;}    // reset lower bound
    int lbound() {return low_bnd;}         // return lower bound
    int ubound() {return ArSize() + low_bnd - 1;} // upper bound
// redefined operators
    double & operator[](int i);
    const double & operator[](int i) const;
};

#endif

