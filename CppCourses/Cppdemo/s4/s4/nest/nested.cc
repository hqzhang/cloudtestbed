// nested.cpp -- use queue having a nested class
// compile along with strng2.cpp
#include <iostream>
using namespace std;
#include "strng2.h"
#include "queuetp.h"

int main()
{
    QueueTP<String> cs(5);
    String temp;

    while(!cs.isfull())
    {
        cout << "Please enter your name. You will be "
                "served in the order of arrival.\n"
                "name: ";
        cin >> temp;
        cs.enqueue(temp);
    }
    cout << "The queue is full. Processing begins!\n";

    while (!cs.isempty())
    {
        cs.dequeue(temp);
        cout << "Now processing " << temp << "...\n";
    }
    return 0;
}

