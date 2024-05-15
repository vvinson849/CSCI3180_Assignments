//do NOT submit or modify this file

#include "ingredient.h"

Ingredient::~Ingredient()
{
}

int Ingredient::getQuality()
{
    return quality;
}

bool Ingredient::isGood()
{
    return quality > 0;
}

void Ingredient::update()
{
    if(quality > 0) //make sure we won't have negative quality values
        quality -= 10;
}
