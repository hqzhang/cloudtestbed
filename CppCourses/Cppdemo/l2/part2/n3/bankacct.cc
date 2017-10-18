// bankacct.cpp -- methods for BankAccount class
#include <iostream>
using namespace std;
#include "bankacct.h"
#include <cstring>

BankAccount::BankAccount(const char *s, long an, double bal)
{
    strncpy(fullName, s, MAX - 1);
    fullName[MAX - 1] = '\0';
    acctNum = an;
    balance = bal;
}
    
void BankAccount::Deposit(double amt)
{
    balance += amt;
}

void BankAccount::Withdraw(double amt)
{
    if (amt <= balance)
        balance -= amt;
    else
        cout << "Withdrawal amount of $" << amt 
            << " exceeds your balance.\n"
            << "Withdrawal canceled.\n";
}

double BankAccount::Balance() const
{
    return balance;
}

void BankAccount::ViewAcct() const
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
