// useover.cpp -- test Overdraft class
// compile with bankacct.cpp and overdrft.cpp
// virtual version
#include <iostream>
using namespace std;
#include "overdrft.h"
const int SIZE = 3;
const int MAX = 35;
inline void EatLine() {while (cin.get() != '\n') continue;}

int main()
{
   BankAccount * baps[SIZE]; 
   char name[MAX];
   long acctNum;
   double balance;
   int acctType;
   int i;
   for (i = 0; i < SIZE; i++)
   {
       cout << "Enter client's name: ";
       cin.get(name,MAX);
       EatLine();
       cout << "Enter client's account number: ";
       cin >> acctNum;
       cout << "Enter client's initial balance: ";
       cin >> balance;
       cout << "Enter 1 for Brass Account, 2 for Brass Plus "
            << "Account: ";
       cin >> acctType;
       EatLine();
       if (acctType == 2)
           baps[i] = new Overdraft(name, acctNum, balance);
       else
       {
           baps[i] = new BankAccount(name, acctNum, balance);
           if (acctType != 1)
               cout << "I'll interpret that as a 1.\n";
       }
    }
    for (i = 0; i < SIZE; i++)
    {
        baps[i]->ViewAcct();
        cout << endl;
    }
    cout << "Bye!\n";
    return 0;

}
