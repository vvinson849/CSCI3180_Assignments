//submit this file

#include "ramenRestaurant.h"
#include "noodle.h"
#include "soup.h"
#include "pork.h"
#include <iostream>
using std::cout;
using std::endl;

RamenRestaurant::RamenRestaurant(int ingredientStorageCapacity) : ingredientStorageCapacity(ingredientStorageCapacity)
{
    ingredientStorage = new Ingredient*[ingredientStorageCapacity]; //create a dynamic array of Ingredient pointers, of size ingredientStorageCapacity
    for(int i=0; i<ingredientStorageCapacity; i++) //we should set all ingredientStorage slots to nullptr's since our storage is literally empty at the beginning
        ingredientStorage[i] = nullptr;
}

RamenRestaurant::~RamenRestaurant()
{
    //TODO: implement this function
    //Hint: use "delete" for non-array dynamic objects, and use "delete []" for dynamic arrays
    for(int i=0; i<ingredientStorageCapacity; ++i)
        if(ingredientStorage[i] != nullptr) {
            delete ingredientStorage[i];
            ingredientStorage[i] = nullptr;
        }
    delete[] ingredientStorage;
    ingredientStorage = nullptr;
}

bool RamenRestaurant::isStorageFull()
{
    return ingredientStorageUsed == ingredientStorageCapacity;
}

void RamenRestaurant::addFoodToStorage(Ingredient* food)
{
    for(int i = 0; i < ingredientStorageCapacity; i++)
    {
        if(ingredientStorage[i] == nullptr) //check if we have found the first non-empty storage slot
        {
            ingredientStorage[i] = food; //add the ingredient to that slot then
            break; //this is important to stop our loop here: what would happen if we don't?
        }
    }
    ingredientStorageUsed++; //update the state
}

bool RamenRestaurant::prepareNoodle(int softness)
{
    if(isStorageFull()) //cannot prepare new noodles when the storage is full
    {
        cout << "Whoops! No more storage space for the new noodle! :(" << endl;
        return false;
    }
    addFoodToStorage(new Noodle(softness)); //prepare it and store it
    cout << "Noodle (" << softness << "% softness) has been prepared and added to storage! :)" << endl;
    return true;
}

bool RamenRestaurant::prepareSoup(int spiciness)
{
    if(isStorageFull()) //cannot prepare new soup when the storage is full
    {
        cout << "Whoops! No more storage space for the new soup! :(" << endl;
        return false;
    }
    addFoodToStorage(new Soup(spiciness)); //prepare it and store it
    cout << "Soup (" << spiciness << "% spiciness) has been prepared and added to storage! :)" << endl;
    return true;
}

bool RamenRestaurant::preparePork()
{
    if(isStorageFull()) //cannot prepare new pork when the storage is full
    {
        cout << "Whoops! No more storage space for the new pork! :(" << endl;
        return false;
    }
    addFoodToStorage(new Pork()); //prepare it and store it
    cout << "Pork has been prepared and added to storage! :)" << endl;
    return true;
}

bool RamenRestaurant::prepareAndServeRamen(int requiredNoodleSoftness, int requiredSoupSpiciness, bool doublePork)
{
    //TODO: implement this function
    /*
        Hint:
        ingredientStorage[i] is merely an Ingredient pointer.
        Therefore, you cannot directly use it like a Noodle pointer even though the object it points to is actually a Noodle object. For example, the following won't compile:
            cout << ingredientStorage[i]->getSoftness() << endl;
        In C++, you can use dynamic_cast for trying to cast it to a Noodle pointer like this:
            Noodle* realNoodlePointer = dynamic_cast<Noodle*>(ingredientStorage[i]);
        The good thing about dynamic_cast is that it gives you a nullptr if the casting fails when ingredientStorage[i] doesn't really point to a Noodle object. It is useful for checking what object it is pointing to. Of course, before you do dynamic_cast, you should make sure ingredientStorage[i] is not really a nullptr itself first.
    */
    /*
        Please copy and use the following for the success message:
        cout << "Ramen has been skillfully prepared and happily served! :)" << endl;

        Please copy and use the following for the failure message:
        cout << "Oh no, we cannot prepare the ramen requested! :(" << endl;
    */
   bool foundNoodle = false, foundSoup = false, found1stPork = false, found2ndPork = !doublePork;
   int noodlePosition, soupPosition, pork1stPosition, pork2ndPosition;

   for(int i=0; i<ingredientStorageCapacity; ++i) {
        if(ingredientStorage[i] == nullptr)
            continue;

        if(!foundNoodle) {
            Noodle* realNoodlePointer = dynamic_cast<Noodle*>(ingredientStorage[i]);
            if(realNoodlePointer != nullptr) {
                if(realNoodlePointer->isGood() && realNoodlePointer->getSoftness()>=requiredNoodleSoftness) {
                    noodlePosition = i;
                    foundNoodle = true;
                }
                continue;
            }
        }

        if(!foundSoup) {
            Soup* realSoupPointer = dynamic_cast<Soup*>(ingredientStorage[i]);
            if(realSoupPointer != nullptr) {
                if(realSoupPointer->isGood() && realSoupPointer->getSpiciness()>=requiredSoupSpiciness) {
                    soupPosition = i;
                    foundSoup = true;
                }
                continue;
            }
        }

        if(!found1stPork || !found2ndPork) {
            Pork* realPorkPointer = dynamic_cast<Pork*>(ingredientStorage[i]);
            if(realPorkPointer != nullptr) {
                if(realPorkPointer->isGood()) {
                    if(!found1stPork) {
                        pork1stPosition = i;
                        found1stPork = true;
                    } else if(pork1stPosition != i) {
                        pork2ndPosition = i;
                        found2ndPork = true;
                    }
                }
                continue;
            }
        }
   }

   if(foundNoodle && foundSoup && found1stPork && found2ndPork) {
        delete ingredientStorage[noodlePosition];
        delete ingredientStorage[soupPosition];
        delete ingredientStorage[pork1stPosition];
        ingredientStorage[noodlePosition] = ingredientStorage[soupPosition] = ingredientStorage[pork1stPosition] = nullptr;
        if(doublePork) {
            delete ingredientStorage[pork2ndPosition];
            ingredientStorage[pork2ndPosition] = nullptr;
            ingredientStorageUsed -= 4;
        } else {
            ingredientStorageUsed -= 3;
        }
        ramenServed++;
        cout << "Ramen has been skillfully prepared and happily served! :)" << endl;
        return true;
   } else {
        cout << "Oh no, we cannot prepare the ramen requested! :(" << endl;
        return false;
   }

}

void RamenRestaurant::update()
{
    //update the states for each of the ingredients stored
    for(int i = 0; i < ingredientStorageCapacity; i++)
        if(ingredientStorage[i] != nullptr) //of course, we have to make sure the current storage slot is not really empty
            ingredientStorage[i]->update(); //so update it
}

void RamenRestaurant::print()
{
    cout << "Currently, we have " << ingredientStorageUsed << " ingredient" << (ingredientStorageUsed!=1?"s":"") << " in the restaurant storage: " << endl;
    for(int i = 0; i < ingredientStorageCapacity; i++) //go through our whole ingredient storage
    {
        cout << "Slot " << i << ": ";
        if(ingredientStorage[i] == nullptr) //nullptr means an empty slot at index i
            cout << "empty";
        else
            ingredientStorage[i]->print(); //invoke the print function for the ingredient - it will call the correct function (Noodle's or Soup's) depending on the actual class of the object the pointer points to: it is a dynamic binding
        cout << endl;
    }
    cout << "And... " << ramenServed << " bowl" << (ramenServed!=1?"s":"") << " of Hellish Teemo Ramen proudly served so far!" << endl;
    cout << endl;
}
