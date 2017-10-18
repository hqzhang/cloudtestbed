
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class adder:public unary_function<int,void>
{
	public:
	adder():sum(0){}
	int sum;

	void operator()(int x){sum+=x;}
};
	
int main()
{
  vector<int> v(10);
  int i;

  for(i=0; i<10; i++) v[i] = 4+i;

  adder result=for_each(v.begin(), v.end(), adder());

  cout << "Contents of v2 after complete copy:\n";
  for(i=0; i<10; i++)
    cout << v[i] << " ";
  cout << "\n\n";
  cout <<"The sum is "<<result.sum<<endl;

  vector<int>::iterator vi=
	  find(v.begin(),v.end(),10);
  if(vi!=v.end()) *vi=50;
  copy(v.begin(),v.end(),ostream_iterator<int> (cout," "));
  cout<<endl;


  return 0;
}
