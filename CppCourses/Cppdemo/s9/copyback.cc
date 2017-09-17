#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <numeric>
#include <iterator>
using namespace std;
int main()
{
vector<int> V(15);
iota(V.begin(), V.end(), 1);
copy_backward(V.begin(), V.begin() + 10, V.begin() + 15);
copy(V.begin(),V.end(),ostream_iterator<int> (cout," "));
cout<<endl;
}
