//do NOT submit or modify this file

#ifndef __NOODLE_H__
#define __NOODLE_H__

#include "ingredient.h"

class Noodle : public Ingredient
{
public:
    Noodle(int softness);
    int getSoftness();
    void update() override;
    void print() override;

private:
    int softness;
};

#endif // __NOODLE_H__