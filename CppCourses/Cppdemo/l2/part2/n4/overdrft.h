// overdrft.h  -- Overdraft class declaration
#ifndef _OVERDRFT_H_
#define _OVERDRFT_H_
#include "bankacct.h"  // modified to use virtual

class Overdraft : public BankAccount
{
private:
    double maxLoan;
    double rate;
    double owesBank;
public:
    Overdraft(const char *s = "Nullbody", long an = -1,
            double bal = 0.0, double ml = 500,
            double r = 0.10);
    Overdraft(const BankAccount & ba, 
            double ml = 500, double r = 0.1);
    void ViewAcct()const;
    void Withdraw(double amt);
    void ResetMax(double m) { maxLoan = m; }
    void ResetRate(double r) { rate = r; };
    void ResetOwes() { owesBank = 0; }
};

#endif
