//
// Created by Imran Essa on 07/11/2018.
//


#include <fstream>
#include <sstream>
#include <string>
#include "../include/Restaurant.h"
#include <iostream>
using namespace std;


std::vector<std::string> Restaurant::readingFileByComma(std::string filePath) {
    vector<std::string> output;
    string input = filePath;
    string s;
    istringstream file(input);
    while(getline(file,s,',')){
        output.push_back(s);
    }
    return output;
}
std::vector<std::string> Restaurant::readingFileBySpace(std::string filePath) {
    vector<string> tables;
    string input = filePath;
    string s;
    istringstream file(input);
    while(getline(file,s,' ')){
        tables.push_back(s);
    }
    return tables;
}

//embty constrcutor
Restaurant::Restaurant():open(false),tables(),menu(),actionsLog() {

}
//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(false),actionsLog() {
    int numOfTables;
    std::string line;
    int count=0;
    ifstream file(configFilePath);
    while(getline(file,line)){
        while(line.length()==0)
            getline(file,line);

        if(line.at(0)!='#'){
            if(count==0){
                numOfTables=std::stoi(line);
                count++;
                vector<Table*> tables(numOfTables);
            }
            else if(count == 1){


                vector<std::string> seats = readingFileByComma(line);
                for (size_t i=0 ; i < seats.size() ; i++){
                    Table* table=new Table(std::stoi(seats.at(i)));
                    tables.push_back(table);
                }
                count++;
            }
            else{
                vector<string> tmp = readingFileByComma(line);
                DishType type;
                if(tmp.at(1) == "VEG")
                    type=VEG;
                else if(tmp.at(1) == "SPC")
                    type=SPC;
                else if(tmp.at(1)=="BVG")
                    type=BVG;
                else
                    type=ALC;
                Dish dish(count-2,tmp.at(0),std::stoi(tmp.at(2)),type);
                menu.push_back(dish);

            }

        }
    }


}

int Restaurant::getNumOfTables() const {
    return tables.size();
}

Table* Restaurant::getTable(int ind) {
   if((ind<0) || ((unsigned)ind > tables.size()))
       return 0;
    return tables.at(ind);
}

std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;

void Restaurant::start() {
    open =true;
    cout << "Restaurant is now open!" <<endl;
    bool itsOk =true;
    int idOfCustomer=0;
    while(itsOk){
        string s;
        getline(cin,s);
        std::vector<std::string> actionToDo = readingFileBySpace(s);
        if(actionToDo.at(0) == "closeall"){
            CloseAll* closeR=new CloseAll();
            (*closeR).act(*this);
            itsOk = false;
        }
        else if(actionToDo.at(0) == "order"){
            Order* order=new Order(std::stoi(actionToDo.at(1)));
            (*order).act(*this);
        }
        else if(actionToDo.at(0)=="move"){
            int src=std::stoi(actionToDo.at(1));
            int dst=std::stoi(actionToDo.at(2));
            int id=std::stoi(actionToDo.at(3));
            MoveCustomer* movecustomer=new MoveCustomer(src , dst , id);
            (*movecustomer).act(*this);
        }
        else if(actionToDo.at(0) == "close"){
            int tableid=std::stoi(actionToDo.at(1));
            Close* closeTable=new Close(tableid);
            closeTable->act(*this);
        }
        else if(actionToDo.at(0) == "menu"){
            PrintMenu* printTheMenu=new PrintMenu();
            printTheMenu->act(*this);
        }
        else if(actionToDo.at(0) == "status"){
            int id=std::stoi(actionToDo.at(1));
            PrintTableStatus* printStatus=new PrintTableStatus(id);
            printStatus->act(*this);
        }
        else if(actionToDo.at(0) == "log"){
            PrintActionsLog* pringLog=new PrintActionsLog();
            pringLog->act(*this);
        }
        else if(actionToDo.at(0) == "backup"){
            BackupRestaurant* backup=new BackupRestaurant();
            backup->act(*this);
        }
        else if(actionToDo.at(0) == "restore"){
            RestoreResturant* restore=new RestoreResturant();
            restore->act(*this);
        }
        else if(actionToDo.at(0) == "open"){
            int tableId=std::stoi(actionToDo.at(1));
            std::vector<Customer*> customersToAdd;
            for(size_t i=2;i<actionToDo.size();i++){
                int indexOfComma=0;
                string input = actionToDo.at(i);
                for(size_t j=0;j<input.size();++j){
                    if(input.at(j)==','){
                        indexOfComma=j;
                    }
                }
                string nameOfCustomer=input.substr(0 , indexOfComma);
                string typeOfCustomer=input.substr((indexOfComma+1));
                if(typeOfCustomer=="veg"){
                    customersToAdd.push_back(new VegetarianCustomer(nameOfCustomer , idOfCustomer));
                }
                else if(typeOfCustomer=="chp"){
                    customersToAdd.push_back(new CheapCustomer(nameOfCustomer , idOfCustomer));
                }
                else if(typeOfCustomer=="spc"){
                    customersToAdd.push_back(new SpicyCustomer(nameOfCustomer , idOfCustomer));
                }
                else if(typeOfCustomer=="alc"){
                    customersToAdd.push_back(new AlchoholicCustomer(nameOfCustomer , idOfCustomer));
                }
                idOfCustomer++;
            }
                OpenTable* openTable=new OpenTable(tableId , customersToAdd);
                openTable->act(*this);
        }

    }

}

void Restaurant::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

void Restaurant::closeRestaurant() {
    open=false;
}

//destructor
Restaurant::~Restaurant() {
    for(size_t i = 0 ; i < tables.size() ; i++){
        delete tables.at(i);
    }
    tables.clear();
    for(size_t j=0;j<actionsLog.size();j++){
        delete actionsLog.at(j);
    }
    actionsLog.clear();
    menu.clear();}



//copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open){
    for(size_t i = 0 ; i< other.tables .size() ; i++){
        tables.push_back(other.tables.at(i)->clone());

    }
    for(size_t i=0;i<other.menu.size();i++){
        menu.push_back(Dish(other.menu.at(i).getId(),other.menu.at(i).getName(),other.menu.at(i).getPrice(),other.menu.at(i).getType()));
    }

    for(size_t k=0; k < other.actionsLog.size();++k){
        actionsLog.push_back(other.actionsLog.at(k)->clone());
    }
}
//operator=

Restaurant& Restaurant::operator=(const Restaurant& other) {
    if( this != &other){
        open=other.open;
        for(size_t i=0;i<other.menu.size();i++){
            menu.push_back(Dish(other.menu.at(i).getId(),other.menu.at(i).getName(),other.menu.at(i).getPrice(),other.menu.at(i).getType()));
        }
        for(size_t i=0 ;i < tables.size() ;i++ ){
            delete tables.at(i);
        }
        tables.clear();
        for(size_t i = 0 ; i< other.tables .size() ; i++){
            tables.push_back(other.tables.at(i)->clone());

        }
        for(size_t j = 0 ; j < actionsLog.size() ; j++){
            delete actionsLog.at(j);
        }
        actionsLog.clear();
        for(size_t k=0; k < other.actionsLog.size();++k){
            actionsLog.push_back(other.actionsLog.at(k)->clone());
        }



    }
    return *this;
}
//move constructor
Restaurant::Restaurant(Restaurant &&other)
{
    open=other.open;
    for(size_t i=0;i<other.tables.size();++i){
        tables.at(i) = other.tables.at(i);
        other.tables.at(i)= nullptr;
    }
    for(size_t i=0;i<other.menu.size();++i){
        menu.push_back(other.menu.at(i));
    }
    for(size_t i=0;i<other.actionsLog.size();++i){
        actionsLog.at(i) = other.actionsLog.at(i);
        other.actionsLog.at(i)=nullptr;
    }}
//move assignment operator

Restaurant& Restaurant::operator=(Restaurant &&other) {
    if(this != &other){
        for(size_t i = 0 ; i < tables.size() ; i++){
            delete tables.at(i);
        }
        tables.clear();
        for(size_t j=0;j<actionsLog.size();j++){
            delete actionsLog.at(j);
        }
        actionsLog.clear();
        menu.clear();
        open=other.open;
        for(size_t i=0;i<other.tables.size();++i){
            tables.at(i) = other.tables.at(i);
            other.tables.at(i)= nullptr;
        }
        for(size_t i=0;i<other.menu.size();++i){
            menu.push_back(other.menu.at(i));
        }
        for(size_t i=0;i<other.actionsLog.size();++i){
            actionsLog.at(i) = other.actionsLog.at(i);
            other.actionsLog.at(i)=nullptr;
        }
    }
    return *this;
}
