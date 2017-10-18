// stacktem.cpp -- test template stack class
// compiler with strng2.cpp
#include <iostream>
using namespace std;
#include <cctype>
#include "stacktp.h"
#include "strng2.h"
int main()
{
    Stack<String> st;   // create an empty stack
    char c;
    String po;
    cout << "Please enter A to add a purchase order,\n"
         << "P to process a PO, or Q to quit.\n";
    while (cin >> c && toupper(c) != 'Q')
    {
        while (cin.get() != '\n')
            continue;
        if (!isalpha(c))
        {
            cout << '\a';
            continue;
        }
        switch(c)
        {
            case 'A':
            case 'a': cout << "Enter a PO number to add: ";
                      cin >> po;
                      if (st.isfull())
                          cout << "stack already full\n";
                      else
                          st.push(po);
                      break;
            case 'P':
            case 'p': if (st.isempty())
                          cout << "stack already empty\n";
                      else {
                          st.pop(po);
                          cout << "PO #" << po << " popped\n";
                          break;
                      }
        }
        cout << "Please enter A to add a purchase order,\n"
             << "P to process a PO, or Q to quit.\n";
    }
    cout << "Bye\n";
    return 0;
}

