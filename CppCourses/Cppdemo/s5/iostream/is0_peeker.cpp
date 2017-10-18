// peeker.cpp -- some istream methods
#include <iostream>
using namespace std;
#include <cstdlib>              // or stdlib.h

int main()
{

//  read and echo input up to a # character
    char ch;

    while(cin.get(ch))          // terminates on EOF
    {
        if (ch != '#')
            cout << ch;
        else
        {
            cin.putback(ch);    // reinsert character               
            break;
        }
    }

    if (!cin.eof())
    {
        cin.get(ch);
        cout << '\n' << ch << " is next input character.\n";
    }
    else
    {
        cout << "End of file reached.\n";
        exit(0);
    }

    while(cin.peek() != '#')    // look ahead
    {
        cin.get(ch);
        cout << ch;
    }
    if (!cin.eof())
    {
        cin.get(ch);
        cout << '\n' << ch << " is next input character.\n";
    }
    else
        cout << "End of file reached.\n";

    return 0;
}
