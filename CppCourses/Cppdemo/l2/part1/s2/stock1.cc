// stock1.cpp           // Stock class methods
#include <iostream>
#include <cstdlib>      // (or stdlib.h) for exit()
#include <cstring>      // (or string.h) for strncpy()
using namespace std;
#include "stock1.h"

// constructors
Stock::Stock()        // default constructor
{
    strcpy(company, "no name");
    shares = 0;
    share_val = 0.0;
    total_val = 0.0;
}

Stock::Stock(const char * co, int n, double pr)
{
    strncpy(company, co, 29);
    company[29] = '\0';
    shares = n;
    share_val = pr;
    set_tot();
}

// class destructor
Stock::~Stock()        // verbose class destructor
{
    cout << "Bye, " << company << "!\n";
}

// other methods
void Stock::buy(int num, double price)
{
    shares += num;
    share_val = price;
    set_tot();
}

void Stock::sell(int num, double price)
{
    if (num > shares)
    {
        cerr << "You can't sell more than you have!\n";
        exit(1);
    }
    shares -= num;
    share_val = price;
    set_tot();
}

void Stock::update(double price)
{
    share_val = price;
    set_tot();
}

void Stock::show()
{
    cout << "Company: " << company
        << "  Shares: " << shares << '\n'
        << "  Share Price: $" << share_val
        << "  Total Worth: $" << total_val << '\n';
}
