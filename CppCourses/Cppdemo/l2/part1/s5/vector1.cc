// vector1.cpp -- methods for Vector class
#include <iostream>
#include <cmath>
using namespace std;
#include "vector1.h"
const double Rad_to_deg = 57.2957795130823;

// private methods
// calculates magnitude from x and y
void Vector::set_mag()
{
    mag = sqrt(x * x + y * y);
}

// calculates angle from x and y
void Vector::set_ang()
{
    ang = atan2(y, x);
}

// public methods
Vector::Vector()    // default constructor
{
    x = y = mag = ang = 0.0;
}

// constructs a Vector from rectangular coordinates
Vector::Vector(double h, double v)
{
    x = h;
    y = v;
    set_mag();
    set_ang();
}

Vector::~Vector()    // destructor
{
}

// sets Vector members from polar coordinates
void Vector::set_by_polar(double m, double a)
{
    mag = m;
    ang = a / Rad_to_deg;
    x = m * cos(ang);
    y = m * sin(ang);
}
 
// shows magnitude and direction of vector
void Vector::show_polar() const
{
    cout << "(" << mag << ", ";
    cout << ang * Rad_to_deg << ")\n";
}

// shows x and y components of vector
void Vector::show_vector() const
{
    cout << "(" << x << ", " << y << ")\n";
}

// operator overloading
// add two Vectors
Vector Vector::operator+(const Vector & b) const
{
    return Vector(x + b.x, y + b.y);
}

// subtract Vector b from a
Vector Vector::operator-(const Vector & b) const
{
    return Vector(x - b.x, y - b.y);
}

// reverse sign of Vector
Vector Vector::operator-() const
{
    return Vector(-x, -y);
}

// multiple vector by n
Vector Vector::operator*(double n) const
{
    return Vector(n * x, n * y);;
}

// friend method
// multiply n by Vector a
Vector operator*(double n, const Vector & a)
{
    return a * n;
}
