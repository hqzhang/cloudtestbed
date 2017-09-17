// vector1.h -- Vector class, introduce operator overloading
#ifndef _VECTOR1_H_
#define _VECTOR1_H_
class Vector
{
private:
    double x;                           // horizontal value
    double y;                           // vertical value
    double mag;                         // length of vector
    double ang;                         // direction of vector
    void set_mag();
    void set_ang();
public:
    Vector();
    Vector(double h, double v);
    ~Vector();
    double xval() const {return x;}     // report x value
    double yval() const {return y;}     // report y value
    double magval() const {return mag;} // report magnitude
    double angval() const {return ang;} // report angle
    void set_by_polar(double m, double a);
    void show_polar() const;            // show polar values
    void show_vector() const;           // show rectilinear values
// operator overloading
    Vector operator+(const Vector & b) const;
    Vector operator-(const Vector & b) const;
    Vector operator-() const;
    Vector operator*(double n) const;
// friend function
    friend Vector operator*(double n, const Vector & a);
};
#endif

