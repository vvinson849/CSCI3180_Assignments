//submit this file

/*
* CSCI3180 Principles of Programming Languages
*
* --- Declaration ---
* For all submitted files, including the source code files and the written  
* report, for this assignment:
*
* I declare that the assignment here submitted is original except for source
* materials explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/
*
*
* Source material acknowledgements (if any):
* 
* Students whom I have discussed with (if any):
*
*/

#include "noodle.h"
#include <iostream>
using std::cout;

Noodle::Noodle(int softness) : softness(softness) //after the ":", we can have a member-initialization list for initializing the data members
{
    //you can also do the following instead of using the member-initialization list:
    //this->softness = softness;  //the LHS must use "this" to differentiate the data member instance variable from the local variable 
}

int Noodle::getSoftness()
{
    return softness;  
}

void Noodle::update()
{
    //TODO: implement this function
    //Hint: you can call the base class implementation of a function by BaseClassName::FunctionName()
    Ingredient::update();
    if(softness > 0)
        softness -= 5;
}

void Noodle::print()
{
    cout << "Noodle (" << getQuality() << "% quality, " << softness << "% softness)";
}

