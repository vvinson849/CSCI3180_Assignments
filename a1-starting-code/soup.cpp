//do NOT submit or modify this file

#include "soup.h"
#include <iostream>
using std::cout;

Soup::Soup(int spiciness) : spiciness(spiciness) //after the ":", we can have a member-initialization list for initializing the data members
{
    //you can also do the following instead of using the member-initialization list:
    //this->spiciness = spiciness;  //the LHS must use "this" to differentiate the data member instance variable from the local variable 
}

int Soup::getSpiciness()
{
    return spiciness;   
}

void Soup::print()
{
    cout << "Soup (" << getQuality() << "% quality, " << spiciness << "% spiciness)";
}