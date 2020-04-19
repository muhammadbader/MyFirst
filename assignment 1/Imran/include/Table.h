//
// Created by Imran Essa on 07/11/2018.
//

#ifndef INC_208953828_206374662_TABLE_H
#define INC_208953828_206374662_TABLE_H

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    ~Table();
    Table(const Table& other);
    Table& operator=(const Table& other);
    Table(Table &&other);
    Table& operator=(Table &&other);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    bool customerIsExist(int id);
    bool tableIsFull();
    std::vector<OrderPair> ordersOfCustomer(int id);
    int getIndexOfCustomer(int id);
    Table* clone();

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList;

};


#endif //INC_208953828_206374662_TABLE_H
