#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <numeric>
#include <iterator>
using namespace std;
int main()
{
  int A[] = {1, 4, 2, 8, 5, 7};
  const int N = sizeof(A) / sizeof(int);
		
  make_heap(A, A+N);
  copy(A, A+N, ostream_iterator<int>(cout, " "));
  cout << endl;
			  
  pop_heap(A,A+N);
  //copy(A,A+N,ostream_iterator<int>(cout," "));
  copy(A,A+N-1,ostream_iterator<int>(cout," "));
  cout<<endl;
  push_heap(A,A+N);
  copy(A,A+N,ostream_iterator<int>(cout," "));
  cout<<endl;
  sort_heap(A, A+N);
  copy(A, A+N, ostream_iterator<int>(cout, " "));
  cout << endl;
}

