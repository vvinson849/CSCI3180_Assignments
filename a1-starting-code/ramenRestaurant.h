#ifndef __RAMEN_RESTAURANT_H__
#define __RAMEN_RESTAURANT_H__

//do NOT submit or modify this file

#include "ingredient.h"

class RamenRestaurant
{
public:
    RamenRestaurant(int ingredientStorageCapacity);
    ~RamenRestaurant();
    bool prepareNoodle(int softness);
    bool prepareSoup(int spiciness);
    bool preparePork();
    bool prepareAndServeRamen(int requiredNoodleSoftness, int requiredSoupSpiciness, bool doublePork);
    void update();
    void print();

private:
    bool isStorageFull();
    void addFoodToStorage(Ingredient* food);

    Ingredient** ingredientStorage;
    int ingredientStorageCapacity;
    int ingredientStorageUsed = 0;
    int ramenServed = 0;
};

#endif // __RAMEN_RESTAURANT_H__