// binary.cpp -- append information to a binary file
#include <iostream> // not required by most systems
using namespace std;
#include <fstream>
#include <iomanip>
#include <cstdlib>  // (or stdlib.h) for exit()

inline void eatline() { while (cin.get() != '\n') continue; }
struct planet
{
    char name[20];      // name of planet
    double population;  // its population
    double g;           // its acceleration of gravity
};

const char * file = "planets.dat";

int main()
{
    planet pl;
    cout.setf(ios::fixed);
    cout.setf(ios::right);

// show initial contents
    ifstream fin;
    fin.open(file, ios::in |ios::binary);   // binary file
    //NOTE: some systems don't accept the ios::binary mode
    if (fin.is_open()) 
    {
    cout << "Here are the current contents of the "
        << file << " file:\n";
    while (fin.read((char *) &pl, sizeof pl))
        {
        cout << setw(20) << pl.name << ": "
              << setprecision(0) << setw(12) << pl.population
              << setprecision(2) << setw(6) << pl.g << "\n";
        }
    }
    fin.close();

// add new data
    ofstream fout(file, ios::out | ios::app | ios::binary);
    //NOTE: some systems don't accept the ios::binary mode
    if (!fout.is_open())
    {
        cerr << "Can't open " << file << " file for output:\n";
        exit(1);
    }

    cout << "Enter planet name (enter a blank line to quit):\n";
    cin.get(pl.name, 20);
    while (pl.name[0] != '\0')
    {
        eatline();
        cout << "Enter planetary population: ";
        cin >> pl.population;
        cout << "Enter planet's acceleration of gravity: ";
        cin >> pl.g;
        eatline();
        fout.write((char *) &pl, sizeof pl);
        cout << "Enter planet name (enter a blank line "
                "to quit):\n";
        cin.get(pl.name, 20);
    }
    fout.close();

// show revised file
    fin.clear();    // not required for some implementations, but won't hurt
    fin.open(file, ios::in | ios::binary);
    if (fin.is_open())
    {
    cout << "Here are the new contents of the "
        << file << " file:\n";
    while (fin.read((char *) &pl, sizeof pl))
        {
        cout << setw(20) << pl.name << ": "
              << setprecision(0) << setw(12) << pl.population
              << setprecision(2) << setw(6) << pl.g << "\n";
        }
    }
    fin.close();

    return 0;
}
