//
// Created by Imran Essa on 07/11/2018.
//


#include "../include/Customer.h"
#include <vector>
#include <iostream>
using namespace std;
Customer::Customer(std::string c_name, int c_id) : name(c_name),id(c_id) {

}
Customer::~Customer() {}
int Customer::getId() const {
    return id;
}
std::string Customer::getName() const {
    return name;
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id): Customer(name,id),errorVeg(true) {

}
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> ordersOfCustomer;
    if(errorVeg){
        bool stop=false;
        int maxIndex=0;
        int max=-1;
        for(size_t i = 0 ; i < menu.size()  ; i++ ){
            if (menu.at(i).getType()==VEG &&!stop){
                ordersOfCustomer.push_back(i);
                stop=true;
            }
            if(menu.at(i).getType()==BVG && menu.at(i).getPrice()>max){
                maxIndex=i;
                max = menu.at(maxIndex).getPrice();
            }

        }
        ordersOfCustomer.push_back(maxIndex);
        if(!stop)
            errorVeg = false;
    }
    return ordersOfCustomer;


}
std::string VegetarianCustomer::getStrategy() const {
    return ("veg");
}

std::string VegetarianCustomer::toString() const {
    string s = "";
    s.append(getName());
    s.append(",");
    s.append(getStrategy());
    return s;
}
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name,id), justOnce(true){

}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> output;
    if(justOnce) {
        int minIndex = 0;
        int min = menu.at(0).getPrice();
        for (size_t i = 1; i < menu.size(); i++) {
            if (menu.at(i).getPrice() < min) {
                minIndex = i;
                min = menu.at(i).getPrice();
            }

        }
        justOnce=false;
        output.push_back(minIndex);
    }
    return output;
}
std::string CheapCustomer::getStrategy() const {
    return ("chp");
}
std::string CheapCustomer::toString() const {
    string s = "";
    s.append(getName());
    s.append(",");
    s.append(getStrategy());
    return s;
}

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name , id),errorSpc(false),stop(false),orderOnce(false){

}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> output;
    if(!errorSpc) {

        int maxIndex;
        int minIndex;
        int max = -1;
        int min=menu.at(0).getPrice();
        if(!orderOnce) {
            for (size_t i = 0; i < menu.size(); ++i) {

                if (menu.at(i).getType() == SPC && menu.at(i).getPrice() > max) {
                    maxIndex = i;
                    max = menu.at(i).getPrice();
                    stop = true;
                }
            }
            orderOnce=true;
            output.push_back(maxIndex);
        }
        else{
            for(size_t i = 0 ; i <menu .size() ; i++){
                if(menu.at(i).getType() == BVG && menu.at(i).getPrice() < min){
                    minIndex=i;
                    min=menu.at(i).getPrice();
                }
            }
            output.push_back(minIndex);
        }
        if(!stop)
            errorSpc=true;


    }
    return output;
}

std::string SpicyCustomer::getStrategy() const {
    return("spc");
}
std::string SpicyCustomer::toString() const {
    string s = "";
    s.append(getName());
    s.append(",");
    s.append(getStrategy());
    return s;
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name , id),idOfAlcDishes(),countAlc(0){

}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu){
    vector<int> output;
    if(countAlc == 0){
        for(size_t i = 0 ; i < menu.size() ; i++){
            if(menu.at(i).getType()==ALC){
                idOfAlcDishes.push_back(i);
            }
        }
        countAlc++;
    }
    if(!(idOfAlcDishes.size()==0)) {

        int nextMinAlcDish = menu.at(idOfAlcDishes.at(0)).getPrice();
        int minIndex = 0;
        for (size_t j = 1; j < idOfAlcDishes.size(); j++) {
            if (menu.at(idOfAlcDishes.at(j)).getPrice() < nextMinAlcDish) {
                minIndex = j;
                nextMinAlcDish = menu.at(idOfAlcDishes.at(j)).getPrice();
            }
            if (menu.at(idOfAlcDishes.at(j)).getPrice() == nextMinAlcDish) {
                if (idOfAlcDishes.at(j) < idOfAlcDishes.at(minIndex))
                    minIndex = j;
            }

        }
        output.push_back(idOfAlcDishes.at(minIndex));
        idOfAlcDishes.erase(idOfAlcDishes.begin() + minIndex);
    }
    return output;

}
std::string AlchoholicCustomer::getStrategy() const {
    return("alc");
}
std::string AlchoholicCustomer::toString() const {
    string s = "";
    s.append(getName());
    s.append(",");
    s.append(getStrategy());
    return s;
}
Customer* VegetarianCustomer::clone() {
    VegetarianCustomer* vg = new VegetarianCustomer(getName(), getId());
    vg->errorVeg=errorVeg;
    return vg;
}
Customer* CheapCustomer::clone() {
    CheapCustomer* chp = new CheapCustomer(getName() , getId());
    chp->justOnce=justOnce;
    return chp;
}
Customer* SpicyCustomer::clone() {
   SpicyCustomer* spc = new SpicyCustomer(getName() , getId());
   spc->errorSpc=errorSpc;
   spc->stop=stop;
   spc->orderOnce=orderOnce;
   return spc;
}
Customer* AlchoholicCustomer::clone() {
    AlchoholicCustomer* alc = new AlchoholicCustomer(getName() , getId());
    alc->idOfAlcDishes=idOfAlcDishes;
    alc->countAlc = countAlc;
    return alc;
}