//
// Created by Imran Essa on 07/11/2018.
//

#include "../include/Dish.h"

Dish::Dish(int id, std::string name, int price, DishType type): id(id), name(name), price(price),type(type){}
int Dish::getId() const {
    return id;
}

Dish& Dish::operator=(const Dish &other){
    Dish(other.id , other.name , other.price , other.type);
    return *this;
}
std::string Dish::getName() const {
    return name;
}
int Dish::getPrice() const {
    return price;
}
DishType Dish::getType() const {
    return type;
}
std::string Dish::enumToString(DishType d) const {
    switch (d) {
        case VEG : return "VEG";
        case BVG : return "BVG";
        case SPC : return "SPC";
        case ALC : return "ALC";

    }
    return "VEG";
}
Dish::~Dish() {}
Dish::Dish(const Dish &other):id(other.id), name(other.name), price(other.price),type(other.type) {}



