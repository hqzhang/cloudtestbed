// stocks.cpp
#include <iostream>
using namespace std;
#include <cstdlib>  // --or stdlib.h--for exit()
#include <cstring>  // --or string.h--for strncpy()

class Stock
{
private:
    char company[30];
    int shares;
    double share_val;
    double total_val;
    void set_tot() { total_val = shares * share_val; }
public:
    void acquire(const char * co, int n, double pr);
    void buy(int num, double price);
    void sell(int num, double price);
    void update(double price);
    void show();
};


void Stock::acquire(const char * co, int n, double pr)
{
    strncpy(company, co, 29); // truncate co to fit if needed
    company[29] = '\0';
    shares = n;
    share_val = pr;
    set_tot();
}

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

int main()
{
    Stock stock1;

    stock1.acquire("NanoSmart", 20, 12.50);
    cout.precision(2);                 // #.## format
    cout.setf(ios::fixed);        // #.## format
    cout.setf(ios::showpoint);    // #.## format
    stock1.show();
    stock1.buy(15, 18.25);
    stock1.show();
    return 0;
}
