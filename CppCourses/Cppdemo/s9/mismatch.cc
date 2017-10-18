#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
int main()
{
int A1[] = { 3, 1, 4, 1, 5, 9, 3 };
int A2[] = { 3, 1, 4, 2, 8, 5, 7 };
const int N = sizeof(A1) / sizeof(int);

pair<int*, int*> result = mismatch(A1, A1 + N, A2);
cout << "The first mismatch is in position " << result.first - A1 << endl;
cout << "Values are: " << *(result.first) << ", " << *(result.second) << endl;
}


