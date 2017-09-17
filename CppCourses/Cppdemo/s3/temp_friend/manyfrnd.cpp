// manyfrnd.cpp -- unbound template friend to a template class
#include <iostream>
using namespace std;

template <typename T>
class ManyFriend
{
private:
    T item;
public:
    ManyFriend(const T & i) : item(i) {}
    template <typename C, typename D> friend void show2(C &, D &);
};

template <typename C, typename D> void show2(C & c, D & d)
{
    cout << c.item << ", " << d.item << endl;
}

int main()
{
    ManyFriend<int> hfi1(10);
    ManyFriend<int> hfi2(20);
    ManyFriend<double> hfd(10.5);
    show2(hfi1, hfi2);
    show2(hfd, hfi2);

    return 0;
}


