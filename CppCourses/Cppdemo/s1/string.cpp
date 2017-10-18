#include <iostream>
using namespace std;

/* This program shows the difference between 
   signed and unsigned integers.
*/
char *name = "BBT CPP";
int main()
{

  char *act= "Learn";
  cout << act << " " << name <<endl;
  name[5]=' ';
  cout <<"Name was changed"<<endl;
  name[6]=' ';
  cout << act << " " << name <<endl;

  return 0;
}
