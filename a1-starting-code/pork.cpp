//do NOT submit or modify this file

#include "pork.h"
#include <iostream>
using std::cout;

Pork::Pork()
{
}

void Pork::print()
{
    cout << "Pork (" << getQuality() << "% quality)";
}