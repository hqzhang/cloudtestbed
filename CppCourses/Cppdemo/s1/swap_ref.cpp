
#include <iostream>
using namespace std;

// Declare swap() using reference parameters.
void swap(int &x, int &y);

int main()
{
  int i, j;

  i = 10;
  j = 20;

  cout << "initial values of i and j: ";
  cout << i << ' ' << j << '\n';
  swap(j, i);
  cout << "swapped values of i and j: ";
  cout << i << ' ' << j << '\n';

  return 0;
}

/* Here, swap() is defined as using call-by-reference,
   not call-by-value. Thus, it can exchange the two
   arguments it is called with.
*/
void swap(int &x, int &y)
{
  int temp;

  temp = x; // save the value at address x
  x = y;    // put y into x
  y = temp; // put x into y
}
