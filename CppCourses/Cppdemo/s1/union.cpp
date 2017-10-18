
// Use a union to exchange the bytes within a short integer.
#include <iostream>
using namespace std;

void disp_binary(unsigned u);

union swap_bytes {
  short int num;
  char ch[2];
};

int main()
{
  swap_bytes sb;
  char temp;

  sb.num = 15;  // binary: 0000 0000 0000 1111


  cout << "Original bytes:  ";
  disp_binary(sb.ch[1]);
  cout << "  ";
  disp_binary(sb.ch[0]);
  cout << "\n\n";


  // exchange the bytes
  temp = sb.ch[0];
  sb.ch[0] = sb.ch[1];
  sb.ch[1] = temp;

  cout << "Exchanged bytes: ";
  disp_binary(sb.ch[1]);
  cout << "  ";
  disp_binary(sb.ch[0]);
  cout << "\n\n";

  return 0;
}

// Display the bits within a byte.
void disp_binary(unsigned u)
{
  register int t;

  for(t=128; t>0; t=t/2)
    if(u & t) cout << "1 ";
    else cout << "0 ";
}

