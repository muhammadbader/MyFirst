#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    User(const std::string& name,const User &other);
    User(const std::string& name, User &&other);
    User &operator= (const User& other )= default;
    User &operator= ( User&& other )= default;
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    void steal(User&);
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual std::string toString() const;
    bool contains(std::vector<Watchable*>* v, Watchable* x);
    void push(Watchable*);
    virtual User* clone()=0;
    virtual void clean();
    virtual void copy(const User&);
    std::vector<int> getwatchedlist()const;

protected:
    std::vector<Watchable*> history;
private:
   const std::string name;
   std:: vector<int> tagsname;
   std::vector <std::string> countertags;
   std::string s;//kind
   std::vector<int> watchedlist;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const std::string& name, const User &other);
    LengthRecommenderUser(const std::string& name,  User &&other);
    LengthRecommenderUser &operator= (const LengthRecommenderUser& other );
    LengthRecommenderUser &operator= ( LengthRecommenderUser&& other );
    virtual Watchable* getRecommendation(Session& s);// we need to build according to the PDF
    virtual  std::string toString() const;
    virtual User* clone();
  
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const std::string& name,const User &other);
    RerunRecommenderUser(const std::string& name, User &&other);
    RerunRecommenderUser &operator= (const RerunRecommenderUser& other );
    RerunRecommenderUser &operator= ( RerunRecommenderUser&& other );
    virtual Watchable* getRecommendation(Session& s);// we need to build according to the PDF
    virtual std::string toString() const;
    virtual User* clone();
   
private:
    int i;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const std::string& name, const User &other);
    GenreRecommenderUser(const GenreRecommenderUser &other);
    GenreRecommenderUser(const std::string& name,  User &&other);
    GenreRecommenderUser &operator= (const GenreRecommenderUser& other );
    GenreRecommenderUser &operator= ( GenreRecommenderUser&& other );
    virtual ~GenreRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);// we need to build according to the PDF
    virtual std::string toString() const;
//    bool contains(std::vector<Watchable*>* v, Watchable* x);
    void insertionSort();
//    virtual std::vector<std::pair<int,std::string>*>* copyfamous() const;
    virtual User* clone();
  
protected:
    int tagfame(Watchable *curr);
private:
    std::vector<std::pair<int,std::string>*> famoustags;
};

#endif
