#include <iostream>
#include <algorithm>
#include <cstring>
#include <ctype.h>
#include <vector>
using namespace std;
inline bool lt_nocase(char c1, char c2) { return tolower(c1) < tolower(c2); }
	
int main()
{
  int A1[] = {1, 3, 5, 7, 9, 11};
  int A2[] = {1, 1, 2, 3, 5, 8, 13};  
  char A3[] = {'a', 'b', 'B', 'B', 'f', 'H'};
  char A4[] = {'A', 'B', 'b', 'C', 'D', 'F', 'F', 'h', 'h'};
		
  const int N1 = sizeof(A1) / sizeof(int);
  const int N2 = sizeof(A2) / sizeof(int); 
  const int N3 = sizeof(A3);
  const int N4 = sizeof(A4);
					
  cout << "Union of A1 and A2: ";
  set_union(A1, A1 + N1, A2, A2 + N2,
      ostream_iterator<int>(cout, " "));
  cout << endl 
      << "Union of A3 and A4: ";
  set_union(A3, A3 + N3, A4, A4 + N4, 
       ostream_iterator<char>(cout, " "),
       lt_nocase);
  cout << endl;
}

