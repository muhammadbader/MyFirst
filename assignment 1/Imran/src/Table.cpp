//
// Created by Imran Essa on 07/11/2018.
//

#include "../include/Table.h"
#include "../include/Dish.h"
#include <vector>
#include <iostream>
using namespace std;
//constructor
Table::Table(int t_capacity): capacity(t_capacity), open(false) {
    customersList=vector<Customer*> {};
    orderList=vector<OrderPair> {};
}
//destructor
Table::~Table() {
    for (size_t i = 0; i < customersList.size(); i++) {
        delete customersList.at(i);
        customersList.at(i) = nullptr;
    }
}
//copy constructor
Table::Table(const Table &other): capacity(other.capacity),open(other.open) {
    for (size_t i = 0; i < other.customersList.size(); i++) {
        customersList.push_back(other.customersList.at(i)->clone());
    }
    for (size_t j = 0; j < other.orderList.size(); j++) {
        orderList.push_back(other.orderList.at(j));

    }
}
//operator=
    Table &Table::operator=(const Table &other) {
        if (this != &other) {
            open = other.open;
            capacity = other.capacity;
            for (size_t i = 0; i < customersList.size(); i++) {
                delete customersList.at(i);
                customersList.at(i) = nullptr;
            }
            customersList.clear();
            orderList.clear();
            for (size_t i = 0; i < other.customersList.size(); i++) {
                customersList.push_back(other.customersList.at(i)->clone());
            }
            for (size_t j = 0; j < other.orderList.size(); j++) {
                orderList.push_back(other.orderList.at(j));
            }

        }
        return *this;
    }
//move constructor
    Table::Table(Table && other)
    {
        capacity = other.capacity;
        open = other.open;
        for (size_t i = 0; i < customersList.size(); i++) {
            customersList.at(i) = other.customersList.at(i);
            other.customersList.at(i) = nullptr;
        }
        for (size_t j = 0; j < orderList.size(); j++) {
            orderList.push_back(other.orderList.at(j));
        }
    }
//move assignment operator
    Table &Table::operator=(Table &&other) {
        if (this != &other) {
            capacity = other.capacity;
            open = other.open;
            for (size_t i = 0; i < customersList.size(); i++) {
                delete customersList.at(i);
                customersList.at(i) = nullptr;
            }
            customersList.clear();
            for (size_t i = 0; i < other.customersList.size(); i++) {
                customersList.at(i) = other.customersList.at(i);
                other.customersList.at(i) = nullptr;
            }

            orderList.clear();
            for (size_t j = 0; j < other.orderList.size(); j++) {
                orderList.push_back(other.orderList.at(j));
            }

        }
        return *this;
    }


    int Table::getCapacity() const {
        return capacity;
    }
    void Table::addCustomer(Customer *customer) {
        if ((!open) && ((unsigned)customersList.size() < (unsigned)capacity)) {
            customersList.push_back(customer);

        }

    }

    int Table::getIndexOfCustomer(int id) {
        int indexOfCustomer;
        for (size_t i = 0; i < customersList.size(); ++i) {
            if (customersList.at(i)->getId() == id) {
                indexOfCustomer = i;
            }
        }
        return indexOfCustomer;

    }

    void Table::removeCustomer(int id) {
        int index = getIndexOfCustomer(id);
        customersList.erase(customersList.begin() + index);

    }

    Customer *Table::getCustomer(int id) {
        return (customersList.at(getIndexOfCustomer(id)));
    }

    std::vector<Customer *> &Table::getCustomers() {
        return customersList;
    }

    std::vector<OrderPair> &Table::getOrders() {
        return orderList;
    }

    void Table::order(const std::vector<Dish> &menu) {
        for (size_t i = 0; i < customersList.size(); i++) {
            vector<int> tmp = customersList.at(i)->order(menu);
            for (size_t k = 0; k < tmp.size(); k++) {
                orderList.push_back(std::make_pair(customersList.at(i)->getId(), menu.at(tmp.at(k))));
                cout << customersList.at(i)->getName() << " ordered " << menu.at(tmp.at(k)).getName() << endl;
            }

        }
    }

    void Table::openTable() {
        open = true;
    }

    void Table::closeTable() {
        open = false;

        for (vector<Customer *>::iterator it = customersList.begin(); it != customersList.end(); ++it) {
            delete (*it);

        }
        customersList.clear();
        orderList.clear();

    }

    int Table::getBill() {
        int bill = 0;
        for (size_t i = 0; i < orderList.size(); ++i) {
            bill = bill + orderList.at(i).second.getPrice();

        }
        return bill;

    }

    bool Table::isOpen() {
        return open;
    }

    bool Table::customerIsExist(int id) {
        bool output = false;
        for (size_t i = 0; (i < customersList.size()) & (!output); ++i) {
            if (customersList.at(i)->getId() == id)
                output = true;
        }
        return output;
    }

    bool Table::tableIsFull() {
        return (unsigned)customersList.size() == (unsigned)getCapacity();
    }

    std::vector<OrderPair> Table::ordersOfCustomer(int id) {
        vector<OrderPair> tmp;
        vector<OrderPair> output;
        for (size_t i = 0; i < orderList.size(); i++) {
            if (orderList.at(i).first == id) {
                output.push_back(std::make_pair(orderList.at(i).first, orderList.at(i).second));
            } else {
                tmp.push_back(orderList.at(i));
            }
        }
        orderList.clear();
        orderList = tmp;
        return output;
    }

    Table *Table::clone() {
        return new Table(*this);
    }



