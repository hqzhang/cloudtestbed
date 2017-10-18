// usebank.cpp
// compile with bankacct.cpp
#include <iostream>
using namespace std;
#include <cstring>
#include "bankacct.h"

int main()
{
    BankAccount Porky("Porcelot Pigg", 381299, 4000.00);
    
    Porky.ViewAcct();
    Porky.Deposit(5000.00);
    cout << "New balance: $" << Porky.Balance() << endl;
    Porky.Withdraw(8000.00);
    cout << "New balance: $" << Porky.Balance() << endl;
    Porky.Withdraw(1200.00);
    cout << "New balance: $" << Porky.Balance() << endl;

    return 0;
}
