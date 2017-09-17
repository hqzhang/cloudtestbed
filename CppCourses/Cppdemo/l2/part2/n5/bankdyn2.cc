// bankdyn.cpp -- methods for BankAccount class
#include <iostream>
using namespace std;
#include "bankdyn.h"
#include <cstring>

BankAccountD::BankAccountD(const char *s, long an, double bal)
{
    fullName = new char[strlen(s) + 1];
    strcpy(fullName, s);
    acctNum = an;
    balance = bal;
}

BankAccountD::BankAccountD(const BankAccountD & ba)
{
    fullName = new char[strlen(ba.fullName) + 1];
    strcpy(fullName, ba.fullName);
    acctNum = ba.acctNum;
    balance = ba.balance;;
}

BankAccountD::~BankAccountD()
{
    delete [] fullName;
}
    
void BankAccountD::Deposit(double amt)
{
    balance += amt;
}

void BankAccountD::Withdraw(double amt)
{
    if (amt <= balance)
        balance -= amt;
    else
        cout << "Withdrawal amount of $" << amt 
            << " exceeds your balance.\n"
            << "Withdrawal canceled.\n";
}

double BankAccountD::Balance() const
{
    return balance;
}

void BankAccountD::ViewAcct() const
{
    // set up ###.## format
    ios::fmtflags initialState =
        cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);
    cout << "Client: " << fullName << endl;
    cout << "Account Number: " << acctNum << endl;
    cout << "Balance: $" << balance << endl;
    cout.setf(initialState); // restore original format 
}

BankAccountD & BankAccountD::operator=(const BankAccountD & ba)
{
    if (this == &ba)
        return *this;
    delete [] fullName;
    fullName = new char[strlen(ba.fullName) + 1];
    strcpy(fullName, ba.fullName);
    acctNum = ba.acctNum;
    balance = ba.balance;
    return *this;
}
