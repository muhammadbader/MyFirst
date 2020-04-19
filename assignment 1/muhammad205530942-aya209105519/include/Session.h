#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "User.h"

class Action;
class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session&);
//    Session(Session&&);//move c'tor
    ~Session();
    Session& operator=(const Session&) ;
//    Session& operator=(Session&&) ;
    void clean();
    void steal(Session& other);
    void copy(const Session&);
    void start();//rule of 3
    std::string& getcurr(int);
    User* getactive()const;
    void setnewActive(User*);
    bool searchuser(int);
    std::unordered_map<std::string,User*>* getMap();
    std::vector<Watchable*> getContent() const;
    void setActionLog(BaseAction *action);
    void setwatching();
    std::vector<BaseAction*> getActionLog() const;

private:
    std::vector<std::string> readingInput(std::string&);
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::vector<std::string> currentAction;
    bool watching;
};
#endif
