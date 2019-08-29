#pragma once
#include<iostream>
using namespace std;


/*主类object*/
class Objects
{
public:
    string show_object(void);
};

string show_object(){
    cout << "Objects are : Bamboos(Blue)\nLandmark(Blue/Red)\n";
    cout << "Color: 0 for Red,1 for Green, 3 for Blue \n";  //敌方颜色，0是红色，1是蓝色。
    return NULL;
}

class Bamboo : public Objects
{
    public:
    int color = 3; //竹子颜色选择3(蓝色);

} ;

class Landmark : public Objects
{
    public:
    int color = 0;
};



