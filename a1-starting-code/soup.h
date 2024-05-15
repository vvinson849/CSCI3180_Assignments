//do NOT submit or modify this file

#ifndef __SOUP_H__
#define __SOUP_H__

#include "ingredient.h"

class Soup : public Ingredient
{
public:
    Soup(int spiciness);
    int getSpiciness();
    void print() override;

private:
    int spiciness;
};

#endif // __SOUP_H__