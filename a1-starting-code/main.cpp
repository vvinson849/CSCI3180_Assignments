//do NOT submit this file

//while you can modify this file to create your own test cases, make sure your submitted files can compile with the original unmodified version of this file

#include <iostream>
#include "ramenRestaurant.h"

using std::cout;
using std::endl;

int main()
{
    cout << "============ 1st hour ============" << endl;
    cout << "Teemo: let's open a ramen restaurant with 10 storage spaces for ingredients!" << endl;
    cout << endl;
    RamenRestaurant teemoRamenRestaurant(10);
    teemoRamenRestaurant.print();
    teemoRamenRestaurant.update();

    cout << "============ 2nd hour ============" << endl;
    cout << "Teemo: let's prepare some ingredients!" << endl;
    cout << "Teemo: a bowl of hard noodle and a bowl of soft noodle!" << endl;
    cout << endl;
    teemoRamenRestaurant.prepareNoodle(10); // note: prepare* functions are boolean functions but it is fine to not use their return values in our simulation; we may check them in the grading test cases though!
    teemoRamenRestaurant.prepareNoodle(70);
    cout << endl;
    teemoRamenRestaurant.print();
    teemoRamenRestaurant.update();

    cout << "============ 3rd hour ============" << endl;
    cout << "Teemo: need some spicy soups and some pork meats!" << endl;
    cout << endl;
    teemoRamenRestaurant.prepareSoup(90);
    teemoRamenRestaurant.prepareSoup(80);
    teemoRamenRestaurant.preparePork();
    teemoRamenRestaurant.preparePork();
    teemoRamenRestaurant.preparePork();
    cout << endl;
    teemoRamenRestaurant.print();
    teemoRamenRestaurant.update();

    cout << "============ 4th hour ============" << endl;
    cout << "Teemo: how about some mild soups and a super soft noodle!" << endl;
    cout << endl;
    teemoRamenRestaurant.prepareSoup(10);
    teemoRamenRestaurant.prepareSoup(20);
    teemoRamenRestaurant.prepareNoodle(100);
    cout << endl;
    teemoRamenRestaurant.print();
    teemoRamenRestaurant.update();

    cout << "============ 5th hour ============" << endl;
    cout << "Teemo: my ramen restaurant is now open!" << endl;
    cout << "Teemo: and we already have a customer!" << endl;
    cout << "Teemo: she wants at least 60% soft noodles and at least 20% spicy soup! double pork as well!" << endl;
    cout << "Teemo: let's prepare a nice bowl of ramen for her!" << endl;
    cout << endl;
    teemoRamenRestaurant.prepareAndServeRamen(60, 20, true);
    cout << endl;
    teemoRamenRestaurant.print();

    for(int i=0; i<98; i++)
        teemoRamenRestaurant.update();
    cout << "============ 98 hours have passed... ============" << endl;
    cout << "Teemo: zzzzzzzzzzzzzzzzzzzz..." << endl;
    cout << endl;
    teemoRamenRestaurant.print();
    
    cout << "============ 103rd hour ============" << endl;
    cout << "Teemo: wow! finally another customer!" << endl;
    cout << "Teemo: let's prepare a nice bowl of ramen for him!" << endl;
    cout << "Teemo: he has no requirements on noodle softness and soup spiciness! single pork!" << endl;
    cout << endl;
    teemoRamenRestaurant.prepareAndServeRamen(0, 0, false);
    cout << endl;
    teemoRamenRestaurant.print();
    teemoRamenRestaurant.update();
    
    cout << "============ 104th hour ============" << endl;
    cout << "Teemo: gg!" << endl;
    cout << "Teemo: maybe instead of pork ramen, I should specialize in mushroom ramen instead next time..." << endl;
    cout << "Teemo: restaurant closes! T_T" << endl;

    return 0;
}