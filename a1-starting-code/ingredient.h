//do NOT submit or modify this file

#ifndef __INGREDIENT_H__
#define __INGREDIENT_H__

class Ingredient 
{
public:
    virtual ~Ingredient();
    int getQuality();
    bool isGood();
    virtual void update();
    virtual void print() = 0; //=0 makes it a pure virtual function, and thus making this class an abstract base class

private:
    int quality = 100;
};

#endif // __INGREDIENT_H__