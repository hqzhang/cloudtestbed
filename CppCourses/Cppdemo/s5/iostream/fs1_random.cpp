// random.cpp -- random access to a binary file
#include <iostream>     // not required by most systems
using namespace std;
#include <fstream>
#include <iomanip>
#include <cstdlib>      // (or stdlib.h) for exit()

struct planet
{
    char name[20];      // name of planet
    double population;  // its population
    double g;           // its acceleration of gravity
};

const char * file = "planets.dat";
inline void eatline() { while (cin.get() != '\n') continue; }

int main()
{
    planet pl;
    cout.setf(ios::fixed);

// show initial contents
    fstream finout;     // read and write streams
    finout.open(file,ios::in | ios::out |ios::binary);
    //NOTE: Some UNIX systems require omitting | ios::binary
    int ct = 0;
    if (finout.is_open()) 
    {
        finout.seekg(0);    // go to beginning
        cout << "Here are the current contents of the "
              << file << " file:\n";
        while (finout.read((char *) &pl, sizeof pl))
        {
            cout << ct++ << ": " << setw(20) << pl.name << ": "
            << setprecision(0) << setw(12) << pl.population
            << setprecision(2) << setw(6) << pl.g << "\n";
        }
        if (finout.eof())
            finout.clear(); // clear eof flag
        else
        {
            cerr << "Error in reading " << file << ".\n";
            exit(1);
        }
    }
    else
    {
        cerr << file << " could not be opened -- bye.\n";
        exit(2);
    }

// change a record
    cout << "Enter the record number you wish to change: ";
    long rec;
    cin >> rec;
    eatline();              // get rid of newline
    if (rec < 0 || rec >= ct)
    {
        cerr << "Invalid record number -- bye\n";
        exit(3);
    }
    streampos place = rec * sizeof pl;  // convert to streampos type
    finout.seekg(place);    // random access
    if (finout.fail())
    {
        cerr << "Error on attempted seek\n";
        exit(4);
    }

    finout.read((char *) &pl, sizeof pl);
    cout << "Your selection:\n";
    cout << rec << ": " << setw(20) << pl.name << ": "
    << setprecision(0) << setw(12) << pl.population
    << setprecision(2) << setw(6) << pl.g << "\n";
    if (finout.eof())
        finout.clear();     // clear eof flag

    cout << "Enter planet name: ";
    cin.get(pl.name, 20);
    eatline();
    cout << "Enter planetary population: ";
    cin >> pl.population;
    cout << "Enter planet's acceleration of gravity: ";
    cin >> pl.g;
    finout.seekp(place);    // go back
    finout.write((char *) &pl, sizeof pl) << flush;
    if (finout.fail())
    {
        cerr << "Error on attempted write\n";
        exit(5);
    }

// show revised file
    ct = 0;
    finout.seekg(0);            // go to beginning of file
    cout << "Here are the new contents of the " << file
         << " file:\n";
    while (finout.read((char *) &pl, sizeof pl))
    {
        cout << ct++ << ": " << setw(20) << pl.name << ": "
              << setprecision(0) << setw(12) << pl.population
               << setprecision(2) << setw(6) << pl.g << "\n";
    }
    finout.close();

    return 0;
}
