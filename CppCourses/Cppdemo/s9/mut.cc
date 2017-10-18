#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <numeric>
#include <iterator>
using namespace std;
	template <class BidirectionalIterator> 
		void snail_sort(BidirectionalIterator first, BidirectionalIterator last)
		{
			  while (prev_permutation(first, last)) {}
		}
	
	int main()
	{
		  int A[] = {8, 3, 6, 1, 2, 5, 7, 4};
		    const int N = sizeof(A) / sizeof(int);
			
			  snail_sort(A, A+N);
			    copy(A, A+N, ostream_iterator<int>(cout, "\n"));
	}
	
	
