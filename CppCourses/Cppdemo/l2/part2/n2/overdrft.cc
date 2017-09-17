// overdrft.cpp -- Overdraft class methods
#include <iostream>
using namespace std;
#include "overdrft.h"

Overdraft::Overdraft(const char *s, long an, double bal,
           double ml, double r) : BankAccount(s, an, bal)
{
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

Overdraft::Overdraft(const BankAccount & ba, double ml, double r)
           : BankAccount(ba)   // uses default copy constructor
{
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

// redefine how ViewAcct() works
void Overdraft::ViewAcct(int n) const
{
    cout << n << endl;
    // set up ###.## format
    ios::fmtflags initialState =
        cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);

    BankAccount::ViewAcct();   // display base portion
    cout << "Maximum loan: $" << maxLoan << endl;
    cout << "Owed to bank: $" << owesBank << endl;
    cout.setf(initialState);
}

// redefine how Withdraw() works
void Overdraft::Withdraw(double amt)
{
    // set up ###.## format
    ios::fmtflags initialState =
        cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);

    double bal = Balance();
    if (amt <= bal)
        BankAccount::Withdraw(amt);
    else if ( amt <= bal + maxLoan - owesBank)
    {
        double advance = amt - bal;
        owesBank += advance * (1.0 + rate);
        cout << "Bank advance: $" << advance << endl;
        cout << "Finance charge: $" << advance * rate << endl;
        Deposit(advance);
        BankAccount::Withdraw(amt);
    }
    else
        cout << "Credit limit exceeded. Transaction cancelled.\n";
    cout.setf(initialState);
}
