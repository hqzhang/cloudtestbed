// setf2.cpp -- use setf() with 2 arguments to control formatting
#include <iostream>
using namespace std;
#include <cmath>

int main()
{
	// use left justification, show the plus sign, show trailing
	// zeros, with a precision of 3
	cout.setf(ios::left, ios::adjustfield);
	cout.setf(ios::showpos);
	cout.setf(ios::showpoint);
	cout.precision(3);
	// use e-notation and save old format setting
    ios::fmtflags old = cout.setf(ios::scientific, ios::floatfield);
	cout << "Left Justification:\n";
	long n;
	for (n = 1; n <= 41; n+= 10)
	{
		cout.width(4);
		cout << n << "|";
		cout.width(12);
		cout << sqrt(n) << "|\n";
	}

	// change to internal justification
	cout.setf(ios::internal, ios::adjustfield);
	// restore default floating-point display style
	cout.setf(old, ios::floatfield);

	cout << "Internal Justification:\n";
	for (n = 1; n <= 41; n+= 10)
	{
		cout.width(4);
		cout << n << "|";
		cout.width(12);
		cout << sqrt(n) << "|\n";
	}

	// use right justification, fixed notation 
	cout.setf(ios::right, ios::adjustfield);
	cout.setf(ios::fixed, ios::floatfield);
	cout << "Right Justification:\n";
	for (n = 1; n <= 41; n+= 10)
	{
		cout.width(4);
		cout << n << "|";
		cout.width(12);
		cout << sqrt(n) << "|\n";
	}
   
	return 0;
}

