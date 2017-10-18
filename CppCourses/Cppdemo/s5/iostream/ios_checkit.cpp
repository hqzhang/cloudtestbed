// check_it.cpp -- validate input
#include <iostream>
using namespace std;
// Note CW users: this version of CW has the non-standard
//                behavior of setting the unread value to 0
int main()
{
    cout.precision(2);
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed);
    cout << "Enter numbers: ";

    double sum = 0.0;
    double input;
    while (cin >> input)
    {
        sum += input;
    }

    cout << "Last value entered = " << input << "\n";
    cout << "Sum = " << sum << "\n";
    return 0;
}

