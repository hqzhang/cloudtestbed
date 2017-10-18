// Overload a function three times.
#include <iostream>
using namespace std;

void f(int i);        // integer parameter
void f(int i, int j); // two integer parameters
void f(double k);     // one double parameter

int main()
{
  f(10);     // call f(int)

  f(10, 20); // call f(int, int)

  f(12.23);  // call f(double)

  return 0;
}

void f(int i)
{
  cout << "In f(int), i is " << i << '\n';
}

void f(int i, int j)
{
  cout << "In f(int, int), i is " << i;
  cout << ", j is " << j << '\n';
}

void f(double k)
{
  cout << "In f(double), k is " << k << '\n';
}

