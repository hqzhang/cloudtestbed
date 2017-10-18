// overdyn2.cpp -- OverdraftD class methods with DMA
#include <iostream>
using namespace std;
#include "overdyn2.h"

OverdraftD::OverdraftD(const char * s, const char * cn,
           long an, double bal,
           double ml, double r) : BankAccountD(s, an, bal)
{
    codeName = new char[strlen(cn) + 1];
    strcpy(codeName, cn);
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

OverdraftD::OverdraftD(const BankAccountD & ba, const char * cn, 
            double ml, double r)
           : BankAccountD(ba)   // uses explicit copy constructor
{
    codeName = new char[strlen(cn) + 1];
    strcpy(codeName, cn);
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

OverdraftD::OverdraftD(const OverdraftD & od) : BankAccountD(od)
{
    codeName = new char[strlen(od.codeName) + 1];
    strcpy(codeName, od.codeName);
    maxLoan = od.maxLoan;
    owesBank = od.owesBank;
    rate = od.rate;
}

OverdraftD::~OverdraftD()
{
    delete [] codeName;
}

// redefine how ViewAcct() works
void OverdraftD::ViewAcct() const
{
    // set up ###.## format
    ios::fmtflags initialState =
        cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);

    BankAccountD::ViewAcct();   // display base portion
    cout << "Code Name: " << codeName << endl;
    cout << "Maximum loan: $" << maxLoan << endl;
    cout << "Owed to bank: $" << owesBank << endl;
    cout.setf(initialState);
}

// redefine how Withdraw() works
void OverdraftD::Withdraw(double amt)
{
    // set up ###.## format
    ios::fmtflags initialState =
        cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);

    double bal = Balance();
    if (amt <= bal)
        BankAccountD::Withdraw(amt);
    else if ( amt <= bal + maxLoan - owesBank)
    {
        double advance = amt - bal;
        owesBank += advance * (1.0 + rate);
        cout << "Bank advance: $" << advance << endl;
        cout << "Finance charge: $" << advance * rate << endl;
        Deposit(advance);
        BankAccountD::Withdraw(amt);
    }
    else
        cout << "Credit limit exceeded. Transaction cancelled.\n";
    cout.setf(initialState);
}

OverdraftD & OverdraftD::operator=(const OverdraftD & od)
{
    if (this == &od)
        return *this;
    BankAccountD::operator=(od);   // base-class assignment
    delete [] codeName;
    codeName = new char[strlen(od.codeName) + 1];
    strcpy(codeName, od.codeName);
    maxLoan = od.maxLoan;
    owesBank = od.owesBank;
    rate = od.rate;
    return *this;
}
