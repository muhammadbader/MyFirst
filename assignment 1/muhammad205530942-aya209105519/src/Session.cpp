        #include <vector>
        #include <unordered_map>
        #include <string>
        #include <fstream>
        #include "Action.h"
        #include "../include/json.hpp"
        #include "Session.h"
        #include "Watchable.h"
        #include "User.h"
        #include <sstream>

        using json=nlohmann::json;
        using namespace std;

        Session::Session(const std::string &configFilePath):content(vector<Watchable*>(0)),actionsLog(vector<BaseAction*>(0)),userMap(unordered_map<string,User*>(0)),activeUser(0),currentAction(vector<string>(0)),watching(true) {
            json j;
            ifstream i(configFilePath);
            i>>j;
            long id=1;
            for(size_t i=0;i<j["movies"].size();i++){
                string name=j["movies"][i]["name"];
                int length=j["movies"][i]["length"];
                vector<string> tags=j["movies"][i]["tags"];
                content.push_back(new Movie(id,name,length,tags));//need movie: move constructor, rule of 5
                id++;
            }

            for(size_t i=0;i<j["tv_series"].size();i++) {
                string name = j["tv_series"][i]["name"];
                int episodelength = j["tv_series"][i]["episode_length"];
                vector<string> tags = j["tv_series"][i]["tags"];
                for (size_t k = 0; k < j["tv_series"][i]["seasons"].size(); k++) {//season
                    for (int m = 1; m <= j["tv_series"][i]["seasons"][k]; m++) {//episode
                     content.push_back(
                            new Episode(id, name, episodelength, k + 1, m, tags));//need Episode: move constructor, rule of 5
                      id++;
                      content[content.size() - 1]->setnextid(id + 1);
                   }
                }
                content[content.size() - 1]->setnextid(0);
            }
            //create a default user on the usermap: "createuser default len"
            string action = "default";
            userMap.insert(
                    {action, new LengthRecommenderUser(action)});// we need to add move constructor (steal) to length recco...
            activeUser = userMap.at(action);
        }

        void Session::copy(const Session& other){

            for(size_t i=0;i<other.content.size();i++){
                    if(other.content[i]->getkind()=="Movie"){
                        Movie *temp=new Movie(other.content[i]->getid(),other.content[i]->getname(),other.content[i]->getlength(),*(other.content[i])->retvectag());
                        content.push_back(temp);
                        temp=nullptr;
                    }else {
                        Episode *temp = new Episode(other.content[i]->getid(), other.content[i]->getname(),
                                                    other.content[i]->getlength(), other.content[i]->getseason(),
                                                    other.content[i]->getepisode(), *(other.content[i])->retvectag());
                        content.push_back(temp);
                        temp = nullptr;
                        //                (*it)->getname(),(*it)->getseason(),(*it)->getepisode(),*other.content[i],
                        //                        other.content[i]->getnextIDepisode())
                    }
                }

                BaseAction *temp2;
                for(auto it=other.actionsLog.begin();it!=other.actionsLog.end();it++){
                    temp2=(*it)->clone();
                    actionsLog.push_back(temp2);
                    temp2=nullptr;
                }

                for(auto it=other.userMap.begin();it!=other.userMap.end();it++){
                    User *temp=it->second;
                    string name=temp->getName();
                    if(temp->toString()=="len"){
                        userMap.insert({name,new LengthRecommenderUser(name,*temp)});

                    }else if(temp->toString()=="gen"){
                        userMap.insert({name,new GenreRecommenderUser(name,*temp)});
                    }else if(temp->toString()=="rer"){
                        userMap.insert({name,new RerunRecommenderUser(name,*temp)});
                    }
                    vector<int> watchedlist=temp->getwatchedlist();
                    User *newuser=userMap.at(name);
                    for(size_t i=0;i<watchedlist.size();i++){
                        newuser->push(content[watchedlist[i]]);
                    }
                }
                activeUser=userMap.at(other.activeUser->getName());

            }
//            Session::Session(Session&& other):content(vector<Watchable*>(0)),actionsLog(vector<BaseAction*>(0)),userMap(unordered_map<string,User*>(0)),activeUser(0),currentAction(vector<string>(0)),watching(true){
//            steal(other);
//        }

            void Session::steal(Session& other){
            activeUser=other.activeUser;
            other.activeUser=nullptr;
                for(size_t i=0;i<other.actionsLog.size();i++){
                    actionsLog.push_back(other.actionsLog[i]);
                    other.actionsLog[i]=nullptr;
                }

                for(size_t i=0;i<other.content.size();i++){
                    content.push_back(other.content[i]);
                    other.content[i]=nullptr;
                }

                for(auto it=other.userMap.begin();it!=other.userMap.end();it++){
                    userMap.insert({it->first,it->second});
                    it->second=nullptr;
                }
            }

            Session::Session(const Session& other):content(vector<Watchable*>(0)),actionsLog(vector<BaseAction*>(0)),userMap(unordered_map<string,User*>(0)),activeUser(0),currentAction(vector<string>(0)),watching(true){
                copy(other);

            }

            Session:: ~Session(){
                clean();
            }

            void Session:: clean(){
                if(!content.empty()){
                    for(size_t i=0;i<content.size();i++){
                        delete content[i];
                        content[i]=nullptr;
                    }
                }
                if(!actionsLog.empty()){
                    for(size_t i=0;i<actionsLog.size();i++){
                        delete actionsLog[i];
                        actionsLog[i]=nullptr;
                    }
                }
                if(!userMap.empty()){
                    activeUser=nullptr;
                    for(auto it=userMap.begin();it!=userMap.end();it++){
                        string fir=it->first;
                        delete userMap.at(fir);
                        userMap.at(fir)=nullptr;
                    }
                }


        }

        Session& Session::operator=(const Session& other) {
            if(this!=&other){
                clean();
                copy(other);
            }
            return *this;
        }

//        Session &Session::operator=( Session&& other) {
//            clean();
//            steal(other);
//            return *this;
//        }

        User* Session::getactive()const{
            return this->activeUser;
        }
        string& Session::getcurr(int i){
            return currentAction[i];
        }

        vector<string> Session::readingInput(string& s){
            string w;
            vector<string> input;
            istringstream parsing(s);
            while(getline(parsing,w,' ')){
                if(w!=""){
                    input.push_back(w);
                }
            }
            return input;
        }

        bool Session::searchuser(int i) {
            unordered_map<string,User*>::const_iterator got=userMap.find(currentAction[i]);
            if(got==userMap.end()) { //if the name does not exists
                return true;
            }else{
                return false;
            }
        }
        std::unordered_map<std::string,User*>* Session::getMap()  {
            return &userMap;
        }
        void Session::setnewActive(User* newActive){
            activeUser=newActive;
        }

        std::vector<Watchable*> Session::getContent() const{
            return content;
        }

        vector<BaseAction*> Session::getActionLog() const{
            return actionsLog;
        }
        void Session::setActionLog(BaseAction *action){
            actionsLog.push_back(action);
        }
        void Session::setwatching()
        {
           watching=!watching;
        }
        void Session::start() {
            cout << "SPLFLIX is now on!" << endl;
           string action={};
            watching = true;
            while (watching) {
                getline(cin, action);
                if(action.size()!=0) {
                    currentAction = readingInput(action);
                    if (currentAction[0] == "createuser") {
                        CreateUser *curr = new CreateUser();//rule of 3
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "changeuser") {//changeuser
                        ChangeActiveUser *curr = new ChangeActiveUser();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "deleteuser") {
                        DeleteUser *curr = new DeleteUser();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "dupuser") {
                        DuplicateUser *curr = new DuplicateUser();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "content") {
                        PrintContentList *curr = new PrintContentList();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "watchhist") {
                        cout << "Watch History for " << activeUser->getName() << endl;
                        PrintWatchHistory *curr = new PrintWatchHistory();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "watch") {
                        string ask = "y";
                        stringstream sid(currentAction[1]);
                        int id;
                        sid >> id;
                        if (static_cast<int>(content.size()) >= id) {
                            while (ask == "y") {
                                //get the current watchable and ask for more
                                Watch *curr = new Watch();
                                curr->act(*this);
                                actionsLog.push_back(curr);
                                curr = nullptr;
                                Watchable *lastwatched = activeUser->get_history()[activeUser->get_history().size() -
                                                                                   1]->getNextWatchable(
                                        *this);//get the last watched episode/movie and the get its reccomendation
                                if (lastwatched != nullptr) {
                                    cout << "We reccomend wathcing " << lastwatched->toString() << ", continue watching? [y/n]"
                                         << endl;
                                    currentAction[1] = to_string(lastwatched->getid());
                                    getline(cin, ask);
                                }
                            }
                        } else {
                            Watch *curr = new Watch();
                            curr->erroract(*this);
                            actionsLog.push_back(curr);
                            curr = nullptr;
                        }
                    } else if (currentAction[0] == "log") {
                        PrintActionsLog *curr = new PrintActionsLog();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    } else if (currentAction[0] == "exit") {
                        Exit *curr = new Exit();
                        curr->act(*this);
                        actionsLog.push_back(curr);
                        curr = nullptr;
                    }
                }
            }

        }

