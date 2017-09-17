// strout.cpp -- incore formatting (output)
#include <iostream>
using namespace std;
#include <sstream> 
#include <string>
int main()
{
    ostringstream outstr;   // manages a string stream
     
    string hdisk;
    cout << "What's the name of your hard disk? ";
    getline(cin, hdisk);
    int cap;
    cout << "What's its capacity in MB? ";
    cin >> cap;
    // write formatted information to string stream
    outstr << "The hard disk " << hdisk << " has a capacity of "
            << cap << " megabytes.\n";  
    string result = outstr.str();   // save result
    cout << result;                 // show contents

    return 0;
}   
