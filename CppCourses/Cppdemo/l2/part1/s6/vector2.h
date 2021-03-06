// vector2.h -- Vector class with <<, mode state
#ifndef _VECTOR2_H_
#define _VECTOR2_H_
class Vector
{
private:   
    double x;          // horizontal value
    double y;          // vertical value
    double mag;        // length of vector
    double ang;        // direction of vector
    char mode;         // 'r' = rectangular, 'p' = polar
    void set_mag();
    void set_ang();
    void set_x();
    void set_y();
public:
    Vector();
    Vector(double n1, double n2, char form = 'r');
    void set(double n1, double n2, char form = 'r');
    ~Vector();
    double xval() const {return x;}         // report x value
    double yval() const {return y;}         // report y value
    double magval() const {return mag;}     // report magnitude
    double angval() const {return ang;}     // report angle
    void polar_mode();
    void rect_mode();
// operator overloading
    Vector operator+(const Vector & b) const;
    Vector operator-(const Vector & b) const;
    Vector operator-() const;
    Vector operator*(double n) const;
// friends
    friend Vector operator*(double n, const Vector & a);
    friend ostream& operator<<(ostream& os, const Vector & v);
};
#endif

