    #include "../include/Session.h"
    #include "User.h"
    #include "Watchable.h"
    #include <string>
    #include <iostream>
    #include <sstream>
    using namespace std;



    BaseAction::BaseAction():errorMsg(""),status(PENDING), kind(""){
    }
    BaseAction::~BaseAction(){
    }

    BaseAction::BaseAction(const BaseAction& other):errorMsg(other.errorMsg),status(other.status), kind(other.kind){//copy c'tor
    }

    ActionStatus BaseAction::getStatus() const{
        return this->status;
    }
    void BaseAction::complete(){
        this->status=COMPLETED;
    }
    string BaseAction::getErrorMsg() const {
        return this->errorMsg;//returns erroMsg in order to print it later
    }

    void BaseAction::error(const std::string &errorMsg) {//need to check this later on
        this->status=ERROR;
        this->errorMsg=errorMsg;//we need to update the errMsg
    }
    void BaseAction::erroract(Session &sess) {
        string s="invalid id";
        error(s);
    }
    void BaseAction::setkind(string k){
        kind=k;
    }

    void BaseAction::setstatus(ActionStatus ss) {
        status=ss;
    }

    void BaseAction::seterror(std::string er) {
        errorMsg=er;
    }

    string BaseAction::geterr() const{
        return errorMsg;
    }
    ActionStatus BaseAction::getsta() const{
        return status;
    }


    //-----------------------------------------------------CreateUser----------------------------------
    CreateUser::CreateUser():BaseAction(){
        setkind("CreateUser");
    }

    CreateUser::CreateUser(const CreateUser& other):BaseAction(other){//copy c'tor
        setkind("CreateUser");
    }

    CreateUser &CreateUser::operator= (const CreateUser& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("CreateUser");
        return *this;

    }

    void CreateUser::act(Session &sess) {
    //if 3 letter is valid

        if(sess.searchuser(1)){ //true=if the user is new user
            if(sess.getcurr(2)=="len"){
                (*sess.getMap()).insert({sess.getcurr(1),new LengthRecommenderUser(sess.getcurr(1))});
                complete();
            }else if(sess.getcurr(2)=="gen"){
                (*sess.getMap()).insert({sess.getcurr(1),new GenreRecommenderUser(sess.getcurr(1))});
                complete();
            }else if(sess.getcurr(2)=="rer"){
                (*sess.getMap()).insert({sess.getcurr(1),new RerunRecommenderUser(sess.getcurr(1))});
                complete();

            }else{
                string s="recommendation algorithm is invalid";
                cout<<"ERROR - "<<s<<endl;
                error(s);
                //invalid 3 letter
                //error(action): change status to ERROR
            }
        }else{//there is a user with the same name, throw error
            string s="username is already in use";
            cout<<"ERROR - "<<s<<endl;
            error(s);

        }

     }

    BaseAction* CreateUser::clone(){
        BaseAction *temp=new CreateUser(*this);
        return temp;
    }
    //toString
    string CreateUser::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="CreateUser COMPLETED";
        else {
            s="CreateUser ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    //--------------------------------------------ChangeActiveUser---------------------------------------
    ChangeActiveUser::ChangeActiveUser():BaseAction(){setkind("ChangeActiveUser");}

    ChangeActiveUser::ChangeActiveUser(const ChangeActiveUser& other):BaseAction(other){//copy c'tor
        setkind("ChangeActiveUser");
    }

    ChangeActiveUser &ChangeActiveUser::operator= (const ChangeActiveUser& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("ChangeActiveUser");
        return *this;

    }

    BaseAction* ChangeActiveUser::clone(){
        BaseAction *temp=new ChangeActiveUser(*this);
        return temp;
    }
    void ChangeActiveUser::act(Session &sess){
        if(sess.searchuser(1)){//the name doesn't exists!!!
            string s="username does not exist";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        } else{
            ///get active and get the new user from the map
            sess.setnewActive((*sess.getMap()).at(sess.getcurr(1)));
            complete();
        }

    }

    string ChangeActiveUser::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="ChangeActiveUser COMPLETED";
        else {
            s="ChangeActiveUser ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    //------------------------------------------------------DeleteUser-------------------------------------
    void DeleteUser::act(Session &sess) {
        if (sess.searchuser(1)) {//the name doesn't exists!!!
            string s="the user does not exists";
            cout<<"ERROR "<<s<<endl;
            error(s);
        }else{//we found the user
            //release from the heap
            delete((*sess.getMap()).at(sess.getcurr(1)));
            //delete from the map
            (*sess.getMap()).erase(sess.getcurr(1));
            complete();
        }
    }

    DeleteUser &DeleteUser::operator= (const DeleteUser& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("DeleteUser");
        return *this;

    }

    BaseAction* DeleteUser::clone(){
        BaseAction *temp=new DeleteUser(*this);
        return temp;
    }
    //toString

    string DeleteUser::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="DeleteUser COMPLETED";
        else {
            s="DeleteUser ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    DeleteUser::DeleteUser():BaseAction(){setkind("DeleteUser");}

    DeleteUser::DeleteUser(const DeleteUser& other):BaseAction(other){//copy c'tor
        setkind("DeleteUser");}
    //-------------------------------------------------DuplicateUser------------------------------------------------------
    void DuplicateUser::act(Session &sess) {

        if(sess.searchuser(1)) {//the first user does not exists
            string s = "Cann't duplicate from a non-existing user";
            cout << "ERROR - " << s << endl;
            error(s);
        }else if (!(sess.searchuser(2))) {//the name does exists!!!
            string s="username is already in use";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        }else{//everything is ok
            string s=(*sess.getMap()).at(sess.getcurr(1))->toString();
            if(s=="len"){
                (*sess.getMap()).insert({sess.getcurr(2),new LengthRecommenderUser(sess.getcurr(2),*((*sess.getMap()).at(sess.getcurr(1))))});
            }else if(sess.getcurr(2)=="gen"){
                (*sess.getMap()).insert({sess.getcurr(2),new GenreRecommenderUser(sess.getcurr(2),*((*sess.getMap()).at(sess.getcurr(1))))});
            }else if(sess.getcurr(2)=="rer"){
                (*sess.getMap()).insert({sess.getcurr(2),new RerunRecommenderUser(sess.getcurr(2),*((*sess.getMap()).at(sess.getcurr(1))))});
            }
            complete();
        }
    }

    DuplicateUser &DuplicateUser::operator= (const DuplicateUser& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("DuplicateUser");
        return *this;

    }

    BaseAction* DuplicateUser::clone(){
        BaseAction *temp=new DuplicateUser(*this);
        return temp;
    }
    //toString
    string DuplicateUser::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="DuplicateUser COMPLETED";
        else {
            s="DuplicateUser ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }
    DuplicateUser::DuplicateUser():BaseAction(){setkind("DuplicateUser");}

    DuplicateUser::DuplicateUser(const DuplicateUser& other):BaseAction(other){//copy c'tor
        setkind("DuplicateUser");
    }
    //-------------------------------------------PrintContentList-----------------------------------------------

    void PrintContentList::act(Session &sess) {
        if (sess.getContent().size() == 0) {
            string s = "there is no content";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        } else {
            vector<Watchable *> content = sess.getContent();
            for (size_t id = 1; id <= (content).size(); id++) {
                Watchable *temp = (content)[id - 1];
                cout << id << ". " << (*temp).toString2() << endl;
                complete();
            }
        }
    }

    PrintContentList &PrintContentList::operator= (const PrintContentList& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("PrintContentList");
        return *this;

    }

    BaseAction* PrintContentList::clone(){
        BaseAction *temp=new PrintContentList(*this);
        return temp;
    }
    //toString
    string PrintContentList::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="PrintContentList COMPLETED";
        else {
            s="PrintContentList ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    PrintContentList::PrintContentList():BaseAction(){setkind("PrintContentList");}

    PrintContentList::PrintContentList(const PrintContentList& other):BaseAction(other){//copy c'tor
        setkind("PrintContentList");;}
    //-----------------------------------------------------PrintWatchHistory-------------------------------------------------
    void PrintWatchHistory::act(Session &sess) {

        if (sess.getContent().size() == 0) {
            string s = "there is no history yet";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        } else {
            vector<Watchable *> history = (*sess.getactive()).get_history();//the users history
            for (size_t id = 1; id <= (history).size(); id++) {
                Watchable *temp = (history)[id - 1];
                cout << id << ". " << (*temp).toString() << endl;
            }
            complete();
        }
    }
    BaseAction* PrintWatchHistory::clone(){
        BaseAction *temp=new PrintWatchHistory(*this);
        return temp;
    }
    //toString
    string PrintWatchHistory::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="PrintWatchHistory COMPLETED";
        else {
            s="PrintWatchHistory ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    PrintWatchHistory &PrintWatchHistory::operator= (const PrintWatchHistory& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("PrintWatchHistory");
        return *this;

    }


        PrintWatchHistory::PrintWatchHistory():BaseAction(){setkind("PrintWatchHistory");}

        PrintWatchHistory::PrintWatchHistory(const PrintWatchHistory& other):BaseAction(other){//copy c'tor
        setkind("PrintWatchHistory");}
    //-------------------------------------------------Watch--------------------------------------------------

    void Watch::act(Session &sess) {

        stringstream sid(sess.getcurr(1));
        int id;
        sid >> id;
        if (id> static_cast<int>(sess.getContent().size())) {
            string s = "id out of bound of content";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        } else {
            Watchable *watching = (sess.getContent())[id - 1];
            cout << "Watching " << watching->toString() << endl;
            sess.getactive()->push(watching);
            complete();
        }
    }

    Watch &Watch::operator= (const Watch& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("Watch");
        return *this;

    }

    BaseAction* Watch::clone(){
        BaseAction *temp=new Watch(*this);
        return temp;
    }
    string Watch::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="Watch COMPLETED";
        else {
            s="Watch ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    Watch::Watch():BaseAction(){setkind("Watch");}

    Watch::Watch(const Watch& other):BaseAction(other){//copy c'tor
        setkind("Watch");}

    //--------------------------------------PrintActionsLog-------------------------------------------
     void PrintActionsLog::act(Session& sess) {

        if (0== sess.getActionLog().size()) {
            string s = "there is no actions yet";
            cout<<"ERROR - "<<s<<endl;
            error(s);
        } else {
            vector<BaseAction *> history = sess.getActionLog();
            for (int i = history.size() - 1; i >= 0; i--) {
                cout << history[i]->toString() << endl;
            }
            complete();
        }
    }

    PrintActionsLog &PrintActionsLog::operator= (const PrintActionsLog& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("PrintActionsLog");
        return *this;

    }

    BaseAction* PrintActionsLog::clone(){
        BaseAction *temp=new PrintActionsLog(*this);
        return temp;
    }

    string PrintActionsLog::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="PrintActionsLog COMPLETED";
        else {
            s="PrintActionsLog ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }

    PrintActionsLog::PrintActionsLog():BaseAction(){setkind("Watch");}

    PrintActionsLog::PrintActionsLog(const PrintActionsLog& other):BaseAction(other){//copy c'tor
        setkind("Watch");}

    //-------------------------------------------------------------Exit---------------------------------------------------
     void Exit::act(Session &sess) {
        sess.setwatching();
        complete();
     }

    Exit &Exit::operator= (const Exit& other ) {
        if (this == &other) {
            return *this;
        }
        seterror(other.geterr());
        setstatus(other.getsta());
        setkind("Exit");
        return *this;

    }

    string Exit::toString() const {
        string s;
        if (getStatus()==COMPLETED)
            s="Exit COMPLETED";
        else {
            s="Exit ERROR: ";
            s.append(getErrorMsg());
        }
        return s;
    }
    BaseAction* Exit::clone(){
        BaseAction *temp=new Exit(*this);
        return temp;
    }


    Exit::Exit():BaseAction(){setkind("Exit");}

    Exit::Exit(const Exit& other):BaseAction(other){//copy c'tor
        setkind("Exit");}

