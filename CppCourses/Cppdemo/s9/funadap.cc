// funadap.cpp -- using function adapters
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
using namespace std;

const int LIM = 5;
int main()
{
    double arr1[LIM] = {36, 39, 42, 45, 48};
    double arr2[LIM] = {25, 27, 29, 31, 33};
    vector<double> gr8(arr1, arr1 + LIM);
    vector<double> m8(arr2, arr2 + LIM);
    ostream_iterator<double> out(cout, " ");
    copy(gr8.begin(), gr8.end(), out);
    cout << endl;
    copy(m8.begin(), m8.end(), out);
    cout << endl;

    transform(gr8.begin(), gr8.end(), m8.begin(), out, plus<double>());
    cout << endl;
 
    transform(gr8.begin(), gr8.end(), out, bind1st(multiplies<double>(), 2.5));
    cout << endl;
  
    return 0;
}
