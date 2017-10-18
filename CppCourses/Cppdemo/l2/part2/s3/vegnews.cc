// vegnews.cpp -- using new and delete with classes
// compile with strng1.cpp

// CODEWARRIOR USERS: CW is one of the compilers that
// immediately runs into the deliberate design flaws of strn1
#include <iostream>
using namespace std;
#include "strng1.h"

String sports("Spinach Leaves Bowl for Dollars");
                            // sports an external object
void callme1();             // creates local object
String * callme2();         // creates dynamic object

int main()
{
    cout << "Top of main()\n";
    String headlines[2] =   // local object array
    {
        String("Celery Stalks at Midnight"),
        String("Lettuce Prey")
    };
    cout << headlines[0] << "\n";
    cout << headlines[1] << "\n";
    callme1();
    cout << "Middle of main()\n";
    String *pr = callme2(); // set pointer to object
    cout << sports << "\n";
    cout << *pr << "\n";    // invoke class method
    delete pr;              // delete object
    cout << "End of main()\n";
    return 0;
}

void callme1()
{
    cout << "Top of callme1()\n";
    String grub;            // local object
    cout << grub << "\n";
    cout << "End of callme1()\n";
}

String * callme2()
{
    cout << "Top of callme2()\n";
    String *pveg = new String("Cabbage Heads Home");
            // dynamic object uses constructor
    cout << *pveg << "\n";
    cout << "End of callme2()\n";
    return pveg;            // pveg expires, object lives
}
