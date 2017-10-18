// bankvirt.h -- a simple BankAccount class with virtual functions
#ifndef _BANKACCT_H_
#define _BANKACCT_H_

class BankAccount
{
private:
    enum {MAX = 35};
    char fullName[MAX];
    long acctNum;
    double balance;
public:
    BankAccount(const char *s = "Nullbody", long an = -1,
                double bal = 0.0);
    void Deposit(double amt);
    virtual void Withdraw(double amt);   // virtual method
    double Balance() const;
    virtual void ViewAcct() const;       // virtual method
};
#endif
