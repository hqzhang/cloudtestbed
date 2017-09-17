#include <iostream>
using namespace std;

int main()
{

  char ca[]= "abc\012\0";
  cout << "Size is " << strlen(ca)<<endl;

  return 0;
}
