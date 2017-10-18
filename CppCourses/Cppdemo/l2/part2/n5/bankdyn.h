// bankdyn.h -- a simple BankAccount class with virtual functions
#ifndef _BANKACCT_H_
#define _BANKACCT_H_

class BankAccountD
{
private:
    char * fullName;
    long acctNum;
    double balance;
public:
    BankAccountD(const char *s = "Nullbody", long an = -1,
                double bal = 0.0);
    BankAccountD(const BankAccountD & ba);
    virtual ~BankAccountD();
    void Deposit(double amt);
    virtual void Withdraw(double amt);   // virtual method
    double Balance() const;
    virtual void ViewAcct() const;       // virtual method
    BankAccountD & operator=(const BankAccountD & ba);
};
#endif
