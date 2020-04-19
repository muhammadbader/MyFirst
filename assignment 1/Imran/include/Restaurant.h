//
// Created by Imran Essa on 07/11/2018.
//

#ifndef INC_208953828_206374662_RESTAURANT_H
#define INC_208953828_206374662_RESTAURANT_H


#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const Restaurant& other);
    Restaurant(const std::string &configFilePath);
    ~Restaurant();
    Restaurant& operator=(const Restaurant& other);
    Restaurant(Restaurant &&other);
    Restaurant& operator=(Restaurant &&other);
    std::vector<std::string> readingFileByComma(std::string filePath);
    std::vector<std::string> readingFileBySpace(std::string filePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const;
    std::vector<Dish>& getMenu();
    void addAction(BaseAction *action);
    void closeRestaurant();


private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};


#endif //INC_208953828_206374662_RESTAURANT_H
