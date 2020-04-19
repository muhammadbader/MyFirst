//
// Created by Imran Essa on 07/11/2018.
//

#ifndef INC_208953828_206374662_CUSTOMER_H
#define INC_208953828_206374662_CUSTOMER_H

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone()=0;
    virtual ~Customer();

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy() const;
    Customer* clone();
private:
    bool errorVeg;
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy() const;
    Customer* clone();
private:
    bool justOnce;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy() const;
    Customer* clone();

private:
    bool errorSpc;
    bool stop;
    bool orderOnce;

};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy() const;
    Customer* clone();
private:
    std::vector<int> idOfAlcDishes;
    int countAlc;
};

#endif //INC_208953828_206374662_CUSTOMER_H
