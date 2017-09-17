// use_vect.cpp -- use the Vector class
// compile with vector1.cpp
#include <iostream>
using namespace std;
#include "vector1.h"
int main()
{
    Vector first_move(120, 50);
    Vector second_move(50, 120);
    Vector result;
    cout.precision(3);                  // format output
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    
    result = first_move + second_move;  // adding objects!
    cout << "First move: ";
    first_move.show_vector();           // display first_move
    cout << "Magnitude, angle = ";
    first_move.show_polar();
    cout << "Second move: ";
    second_move.show_vector();
    cout << "Magnitude, angle = ";
    second_move.show_polar();
    cout << "Result: ";
    result.show_vector();
    cout << "Magnitude, angle = ";
    result.show_polar();
    
    Vector twotimes = result * 2.0;     // member function
    cout << "Doubled result: ";
    twotimes.show_vector();

    result = 0.5 * result;              // friend function
    cout << "Halved result: ";
    result.show_vector();
   
    return 0;
}

