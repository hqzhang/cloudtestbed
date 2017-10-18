// usedyn2.cpp -- test OverdraftD class
// compile with bankdyn.cpp and overdyn2.cpp
#include <iostream>
using namespace std;
#include "overdyn2.h"

int main()
{
    BankAccountD dolly("Dahlia Dahl", 453216, 6000);
    BankAccountD temp;
    temp = dolly;
    temp.ViewAcct();
    cout << endl;
    OverdraftD rolly("Roland Rayleigh", "Rocky", 223391, 8000);
    OverdraftD dup;
    dup = rolly;
    dup.ViewAcct();
    cout << endl;
    dup = dolly;
    dup.ViewAcct();
    cout << "Bye!\n";
    return 0;
}
