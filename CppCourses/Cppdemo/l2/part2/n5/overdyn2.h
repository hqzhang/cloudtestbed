// overdyn2.h  -- OverdraftD class declaration with DMA
#ifndef _OVERDRFT_H_
#define _OVERDRFT_H_
#include "bankdyn.h"

class OverdraftD : public BankAccountD
{
private:
    double maxLoan;
    double rate;
    double owesBank;
    char * codeName;                                // new
public:
    OverdraftD(const char * s = "Nullbody", const char *cn = "cent",
            long an = -1, double bal = 0.0, double ml = 500,
            double r = 0.10);
    OverdraftD(const BankAccountD & ba,const char * cn = "cent", 
            double ml = 500, double r = 0.1);
    OverdraftD(const OverdraftD & od);              // new
    ~OverdraftD();                                  // new
    void ViewAcct()const;
    void Withdraw(double amt);
    void ResetMax(double m) { maxLoan = m; }
    void ResetRate(double r) { rate = r; };
    void ResetOwes() { owesBank = 0; }
    OverdraftD & operator=(const OverdraftD & od);  // new
};

#endif
