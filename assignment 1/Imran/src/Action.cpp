//
// Created by Imran Essa on 07/11/2018.
//
 #include "../include/Restaurant.h"
#include "../include/Action.h"
#include <iostream>
using namespace std;
extern Restaurant* backup;

BaseAction::BaseAction() {
    status = PENDING;
    errorMsg="";
}

void BaseAction::complete() {
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    cout <<  "Error: " << errorMsg << endl;
}
ActionStatus BaseAction::getStatus() const {
    return status;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
std::string BaseAction::printStatus() const {
    string output="";
    if(getStatus()==COMPLETED){
        output.append("Completed");
    }
    else if(getStatus()==ERROR)
        output.append("Error: "+ getErrorMsg());
    return  output;
}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction() ,  tableId(id) , customers(customersList) {
}

void OpenTable::act(Restaurant &restaurant) {
    Table* mytable= restaurant.getTable(tableId);
    if(mytable==0||(*mytable).isOpen() )
        error("Table does not exist or is already open");
    else {
        for(size_t i = 0 ; i < customers.size() ; i++){
            (*mytable).addCustomer(customers.at(i)->clone());
        }
        mytable->openTable();
        complete();

    }
    restaurant.addAction(this);


}
std::string OpenTable::toString() const {
    string s="open " + std::to_string(tableId);
    s.append(" ");
    for(size_t i=0 ; i<customers.size();++i){
        s.append(customers.at(i)->toString()+" ");
    }
    s.append(printStatus());
    return s;

}

OpenTable::~OpenTable() {
    for(size_t i = 0 ; i < customers.size() ; i++){
        delete customers.at(i);

    }
}
OpenTable::OpenTable(const OpenTable &other): BaseAction() , tableId(other.tableId) {
    for (size_t i = 0; i < other.customers.size(); i++) {
        customers.push_back(other.customers.at(i)->clone());
    }
}
Order::Order(int id) :BaseAction() ,tableId(id){}

void Order::act(Restaurant &restaurant) {
    Table* mytable= restaurant.getTable(tableId);
    if(mytable==0||!(*mytable).isOpen()){
        error("Table does not exist or is not open");
    }

   else{
        (*mytable).order(restaurant.getMenu());
        complete();
    }
    restaurant.addAction(this);

}
std::string Order::toString() const {
    string s="order "+std::to_string(tableId);
    s.append(" ");
    s.append(printStatus());
    return s;
}
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction() ,srcTable(src) , dstTable(dst) , id(customerId) {}
void MoveCustomer::act(Restaurant &restaurant) {
    Table* sourceTable = restaurant.getTable(srcTable);
    Table* destTable = restaurant.getTable(dstTable);
    if(sourceTable==0 || destTable==0 || ! (sourceTable->customerIsExist(id)))
        error("Cannot move customer");
    else if(destTable->tableIsFull()){
        error("Cannot move customer");
    }
    else if(! destTable->isOpen() | ! sourceTable->isOpen()){
        error("Cannot move customer");
    }
    else if(srcTable!=dstTable) {
        if (sourceTable->getCustomers().size() == 1) {
            destTable->getCustomers().push_back(sourceTable->getCustomers().at(0));
            vector<OrderPair> customerOrders = sourceTable->ordersOfCustomer(id);//returns the vector orders of the customer and delete them from the source table vector
            for (size_t i = 0; i < customerOrders.size(); ++i) {
                destTable->getOrders().push_back(
                        std::make_pair(customerOrders.at(i).first, customerOrders.at(i).second));
            }
            Close *closeSrcTable = new Close(srcTable);
            closeSrcTable->act(restaurant);
            delete closeSrcTable;
            closeSrcTable = nullptr;
            complete();
        }
        else {
            vector<OrderPair> customerOrders = sourceTable->ordersOfCustomer(id);//returns the vector orders of the customer and delete them from the source table vector
            for (size_t i = 0; i < customerOrders.size(); ++i) {
                destTable->getOrders().push_back(
                        std::make_pair(customerOrders.at(i).first, customerOrders.at(i).second));
            }
            for (size_t j = 0; j < customerOrders.size(); ++j) {
                customerOrders.erase(customerOrders.begin() + j);
            }
            customerOrders.clear();
            destTable->getCustomers().push_back(sourceTable->getCustomer(id));
            sourceTable->removeCustomer(id);
            complete();
        }
    }
    if(srcTable==dstTable)
        complete();
    restaurant.addAction(this);

}
std::string MoveCustomer::toString() const {
    string s="move " + std::to_string(srcTable);
    s.append(" " );
    s=s + std::to_string(dstTable);
    s.append(" "+std::to_string(id));
    s.append(" "+printStatus());
    return  s;
}

Close::Close(int id):BaseAction() , tableId(id) {}

void Close::act(Restaurant &restaurant) {

    Table* mytable= restaurant.getTable(tableId);

    if(mytable==0||!(*mytable).isOpen())
        error("Table does not exist or is not open");
    else{
        cout << "Table " << tableId << " was closed. Bill " << (*mytable).getBill() << "NIS" << endl;
        (*mytable).closeTable();
        complete();

    }
    restaurant.addAction(this);

}
std::string Close::toString() const {

    return "close " + std::to_string(tableId) + printStatus();
}
CloseAll::CloseAll(): BaseAction(){}
void CloseAll::act(Restaurant &restaurant) {
    for( int i = 0 ; i < restaurant.getNumOfTables() ; i++){
        Table* table=restaurant.getTable(i);
        if((table->isOpen())){
            Close* closeTable=new Close(i);
            closeTable->act(restaurant);
        }
    }
    complete();
    restaurant.closeRestaurant();
    restaurant.addAction(this);
}
std::string CloseAll::toString() const {
    string s;
    s.append("closeall ");
    s.append("Completed");
    return s;
}
PrintMenu::PrintMenu():BaseAction()  {}
void PrintMenu::act(Restaurant &restaurant) {

    for(size_t i=0 ; i < restaurant.getMenu().size() ; i++){
        cout << restaurant.getMenu().at(i).getName() << " "<<restaurant.getMenu().at(i).enumToString(restaurant.getMenu().at(i).getType()) <<" "<< restaurant.getMenu().at(i).getPrice() << "NIS" <<endl;
    }
    complete();
    restaurant.addAction(this);


}
std::string PrintMenu::toString() const {
    string s;
    s.append("menu ");
    s.append("Completed");
    return s;
}

PrintTableStatus::PrintTableStatus(int id): BaseAction() ,tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *mytable = restaurant.getTable(tableId);
    if (mytable != 0) {
        if (!(mytable->isOpen()))
            cout << "Table " << tableId << " status: closed" << endl;
        else {
            cout << "Table " << tableId << " status: open" << endl;
            cout << "Customers:" << endl;
            for (size_t i = 0; i < mytable->getCustomers().size(); i++) {
                cout << mytable->getCustomers().at(i)->getId() << " " << mytable->getCustomers().at(i)->getName()
                     << endl;
            }
            cout << "Orders:" << endl;
            for (size_t j = 0; j < mytable->getOrders().size(); j++) {
                cout << mytable->getOrders().at(j).second.getName() << " "
                     << mytable->getOrders().at(j).second.getPrice() << "NIS " << mytable->getOrders().at(j).first
                     << endl;
            }
            cout << "Current bill: " << mytable->getBill() << "NIS" << endl;
        }
        complete();
        restaurant.addAction(this);

    }
}
std::string PrintTableStatus::toString() const {
    string s;
    s.append("status "+std::to_string(tableId));
    s.append(" Completed");
    return s;
}

PrintActionsLog::PrintActionsLog():BaseAction() {}
void PrintActionsLog::act(Restaurant &restaurant) {

    vector<BaseAction*> doneactions = restaurant.getActionsLog();
    for(size_t i=0;i<doneactions.size();++i){
        if((doneactions.at(i)->toString().compare("log Completed") != 0 ))
              cout << doneactions.at(i)->toString() << endl;
    }
    restaurant.addAction(this);
}
std::string PrintActionsLog::toString() const {
    string s;
    s.append("log ");
    s.append("Completed");
    return s;
}
BackupRestaurant::BackupRestaurant():BaseAction() {}
void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup != nullptr) {
        backup = &restaurant;
    }
else{
        backup=new Restaurant(restaurant);

    }
    complete();
    restaurant.addAction(this);
}
std::string BackupRestaurant::toString() const {
    string s;
    s.append("backup ");
    s.append("Completed");
    return s;
}
RestoreResturant::RestoreResturant():BaseAction(){}
void RestoreResturant::act(Restaurant &restaurant) {
    if(backup== nullptr)
        error("No backup available");
    else{
        restaurant=*backup;
        complete();
    }
    restaurant.addAction(this);

}
std::string RestoreResturant::toString() const {
    string s="";
    s.append("restore "+ printStatus());
    return s;
}
void BaseAction::setStatus(ActionStatus status) {
    this->status=status;
}


BaseAction* OpenTable::clone() {
   BaseAction* base=new OpenTable(*this);
   base->setStatus(this->getStatus());
   return base;
}
BaseAction* Order::clone() {
    return new Order(*this);
}
BaseAction* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}
BaseAction* Close::clone() {
    return new Close(*this);
}
BaseAction* CloseAll::clone() {
    return new CloseAll(*this);
}
BaseAction* PrintMenu::clone() {
    return new PrintMenu(*this);
}
BaseAction* PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}
BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}
BaseAction* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}
BaseAction* RestoreResturant::clone() {
    return new RestoreResturant(*this);
}
BaseAction::~BaseAction() {

}

