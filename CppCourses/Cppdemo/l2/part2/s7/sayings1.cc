// sayings1.cpp -- uses expanded string class
// compile with strng2.cpp
#include <iostream>
using namespace std;
#include "strng2.h"
const int ArSize = 10;
const int MaxLen =81;
int main()
{
    String name;
    cout <<"Hi, what's your name?\n>> ";
    cin >> name;
    
    cout << name << ", please enter up to " << ArSize
        << " short sayings <empty line to quit>:\n";
    String sayings[ArSize];     // array of objects
    char temp[MaxLen];          // temporary string storage
    int i;
    for (i = 0; i < ArSize; i++)
    {
        cout << i+1 << ": ";
        cin.get(temp, MaxLen);
        while (cin && cin.get() != '\n')
            continue;
        if (!cin || temp[0] == '\0')    // empty line?
            break;              // i not incremented
        else
            sayings[i] = temp;  // overloaded assignment
    }
    int total = i;              // total # of lines read

    cout << "Here are your sayings:\n";
    for (i = 0; i < total; i++)
        cout << sayings[i] << "\n";

    int shortest = 0;
    int first = 0;
    for (i = 1; i < total; i++)
    {
        if (sayings[i].length() < sayings[shortest].length())
            shortest = i;
        if (sayings[i] < sayings[first])
            first = i;
    }
    cout << "Shortest saying:\n" << sayings[shortest] << "\n";
    cout << "First alphabetically:\n" << sayings[first] << "\n";
    return 0;
}

