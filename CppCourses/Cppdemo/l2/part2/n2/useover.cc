// useover.cpp -- test Overdraft class
// compile with bankacct.cpp and overdrft.cpp
#include <iostream>
using namespace std;
#include "overdrft.h"

int main()
{
    BankAccount Porky("Porcelot Pigg", 381299, 4000.00);
// convert Porcelot to new account type
    Overdraft Porky2(Porky);
    Porky2.ViewAcct(1);
    cout << "Depositing $5000:\n";
    Porky2.Deposit(5000.00);
    cout << "New balance: $" << Porky2.Balance() << "\n\n";
    cout << "Withdrawing $8000:\n";
    Porky2.Withdraw(8000.00);
    cout << "New balance: $" << Porky2.Balance() << "\n\n";
    cout << "Withdrawing $1200:\n";
    Porky2.Withdraw(1200.00);
    Porky2.ViewAcct(3);
    cout << "\nWithdrawing $500:\n";
    Porky2.Withdraw(500.00);
    Porky2.ViewAcct(3);
    Porky2.ResetOwes();
    cout << endl;
    Porky2.ViewAcct(4);

    return 0;
}
