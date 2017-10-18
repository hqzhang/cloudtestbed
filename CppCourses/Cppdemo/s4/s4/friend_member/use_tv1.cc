//use_tv1.cpp
#include <iostream>
using namespace std;
#include "tvfm.h"  // use friend member version

int main(void)
{
    Tv s20;
    cout << "Initial settings for 20\" TV:\n";
    s20.settings();
    s20.onoff();
    s20.chanup();
    cout << "\nAdjusted settings for 20\" TV:\n";
    s20.settings();

    Remote grey;

    grey.set_chan(s20, 10);
    grey.volup(s20);
    grey.volup(s20);
    cout << "\n20\" settings after using remote:\n";
    s20.settings();

    Tv s27(Tv::On);
    s27.set_mode();
    grey.set_chan(s27,28);
    cout << "\n27\" settings:\n";
    s27.settings();

    return 0;
}

