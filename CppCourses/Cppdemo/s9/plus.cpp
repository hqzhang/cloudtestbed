#include <iterator>
#include <functional>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <numeric>
using namespace std;
int main(){
	
	const int N = 10;
	vector<double> V1(N);
	vector<double> V2(N);
	vector<double> V3(N);

	iota(V1.begin(), V1.end(), 1);
	fill(V2.begin(), V2.end(), 75);

	assert(V2.size() >= V1.size() && V3.size() >= V1.size());
	transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),
    	plus<double>());
	ostream_iterator<double> output(cout," ");
	copy(V1.begin(),V1.end(),output);
	cout<<endl;
	copy(V2.begin(),V2.end(),output);
	cout<<endl;
	copy(V3.begin(),V3.end(),output);
	return 0;
}
