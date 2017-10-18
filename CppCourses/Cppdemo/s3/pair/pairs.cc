// pairs.cpp -- define and use a Pair template
#include <iostream>
using namespace std;

template <class T1, class T2>
class Pair
{
private:
    T1 a;
    T2 b;
public:
    T1 & first(const T1 & f);
    T2 & second(const T2 & s);
    T1 first() const { return a; }
    T2 second() const { return b; }
    Pair(const T1 & f, const T2 & s) : a(f), b(s) { }
};

template<class T1, class T2>
T1 & Pair<T1,T2>::first(const T1 & f)
{
    a = f;
    return a;
}
template<class T1, class T2>
T2 & Pair<T1,T2>::second(const T2 & s)
{
    b = s;
    return b;
}

int main()
{
    Pair<char *, int> ratings[4] =
    {
        Pair<char *, int>("The Purple Duke", 5),
        Pair<char *, int>("Jake's Frisco Cafe", 4),
        Pair<char *, int>("Mont Souffle", 5),
        Pair<char *, int>("Gertie's Eats", 3)
    };

    int joints = sizeof(ratings) / sizeof (Pair<char *, int>);
    cout << "Rating:\t Eatery\n";
    for (int i = 0; i < joints; i++)
        cout << ratings[i].second() << ":\t "
             << ratings[i].first() << "\n";

    ratings[3].second(6);
    cout << "Oops! Revised rating:\n";
    cout << ratings[3].second() << ":\t " 
         << ratings[3].first() << "\n";

    return 0;
}


