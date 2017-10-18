// excptinh.cpp -- use the ArrayDbE and LimitArE classes
// Compile with arraydbe.cpp, limarre.h
#include <iostream>
using namespace std;
#include "arraydbe.h"
#include "limarre.h"

const int Years = 4;
const int FirstYear = 1998;
int main()
{
    int year;
    double total = 0;
    try {
        LimitArE income(Years, FirstYear);
        ArrayDbE busywork(Years);
        cout << "Enter your income for the last " << Years
                << " years:\n";
        for (year = FirstYear; year < FirstYear + Years; year++)
        {
             cout << "Year " << year << ": $";
             cin >> income[year];
             busywork[year - FirstYear] = 0.2 * income[year];
        }
        cout.precision(2);
        cout.setf(ios::showpoint);
        cout.setf(ios::fixed,ios::floatfield);
        cout << "Recapitulating, here are the figures:\n";
        for (year = FirstYear; year <= FirstYear + Years; year++)
        {
            cout << year << ": $" << income[year] << "\n";
            total += income[year];
        }
        cout << "busywork values: " << busywork;
    }   // end of try block
    catch (LimitArE::SonOfBad & bi)   // 1st handler
    {
        cout << "LimitArE exception: "
                << bi.badindex << " is a bad index value\n";
        cout << "Index should be in the range " << bi.l_lim
                << " to " << bi.u_lim << ".\n";
    }
    catch (LimitArE::BadIndex & bi)   // 2nd handler
    {
        cout << "ArrayDbE exception: "
                << bi.badindex << " is a bad index value.\n";
    }
    cout << "Total income for " << (year - FirstYear)
            << " years is $" << total << ".\n";
    cout << "Bye!\n";
    return 0;
}

