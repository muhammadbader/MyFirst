//
// Created by Imran Essa on 07/11/2018.
//

#ifndef INC_208953828_206374662_DISH_H
#define INC_208953828_206374662_DISH_H

#include <string>

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    Dish& operator=(const Dish& other);
    DishType getType() const;
    std::string enumToString(DishType d) const;
    ~Dish();
    Dish(const Dish& other);

private:
    const int id;
    const std::string name;
    const int price;
    const DishType type;

};



#endif //INC_208953828_206374662_DISH_H
