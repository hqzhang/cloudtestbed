// exceptar.cpp -- use the ArrayDbE class
// Compile with arraydbe.cpp
#include <iostream>
using namespace std;
#include "arraydbe.h"

const int Players = 5;
int main()
{
    try {
        ArrayDbE Team(Players);
        cout << "Enter free-throw percentages for your 5 "
                "top players as a decimal fraction:\n";
        int player;
        for (player = 0; player < Players; player++)
        {
             cout << "Player " << (player + 1) << ": % = ";
             cin >> Team[player];
        }
        cout.precision(1);
        cout.setf(ios::showpoint);
        cout.setf(ios::fixed,ios::floatfield);
        cout << "Recapitulating, here are the percentages:\n";
        for (player = 0; player <= Players; player++)
            cout << "Player #" << (player + 1) << ": "
                    << 100.0 * Team[player] << "%\n";
    }                                   // end of try block
    catch (ArrayDbE::BadIndex & bi)     // start of handler
    {
        cout << "ArrayDbE exception: "
                << bi.badindex << " is a bad index value\n";
    }                                   // end of handler
    cout << "Bye!\n";
   return 0;
}


