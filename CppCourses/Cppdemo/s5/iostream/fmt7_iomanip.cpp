// iomanip.cpp -- use manipulators from iomanip
// some systems require explicitly linking the math library
#include <iostream>
using namespace std;
#include <iomanip>
#include <cmath>

int main()
{
    // use new standard manipulators
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed);
    cout.setf(ios::right);

    // use iomanip manipulators
    cout << setw(6) << "N" << setw(14) << "square root"
         << setw(15) << "fourth root\n";

    double root;
    for (int n = 10; n <=100; n += 10)
    {
        root = sqrt(n);
        cout << setw(6) << setfill('.') << n << setfill(' ')
               << setw(12) << setprecision(3) << root
               << setw(14) << setprecision(4) << sqrt(root)
               << "\n";
    }

    return 0;
}
