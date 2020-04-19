    #include <vector>
    #include <string>
    #include <unordered_set>
    #include "User.h"
    #include "Watchable.h"
    #include "Session.h"
    #include <algorithm>
    using namespace std;


    User::User(const string& name_):history(vector<Watchable*>(0)), name(name_),tagsname(vector<int>(0)),countertags(vector<string>(0)),s(""),watchedlist(vector<int>(0)){}

    User::User(const string &name_, const User &other):history(vector<Watchable*>(0)), name(name_),tagsname(other.tagsname),countertags(other.countertags),s(other.s),watchedlist(other.watchedlist){
           copy(other);
    }

    User::User(const string &name_,  User &&other):history(vector<Watchable*>(0)), name(name_),tagsname(vector<int>(0)),countertags(vector<string>(0)),s(""),watchedlist(vector<int>(0)){
        steal(other);
    }
    User::~User() {
        clean();
    }


    string User::getName() const {
        return name;
    }
    vector<Watchable*> User::get_history() const{
        return history;
    }


    bool User:: contains(vector<Watchable*>* v, Watchable* x){
        if ((*v).empty())
            return false;
        if ( find((*v).begin(), (*v).end(), x)!= (*v).end())
            return true;
        else
            return false;
    }
    string User::toString() const{
        return "User";
    }

    void User::steal(User& other){
        s=other.s;
        for(size_t i=0;i<other.history.size();i++){
            history.push_back(other.history[i]);
            watchedlist.push_back(other.watchedlist[i]);
            other.history[i]=nullptr;
        }
        for(size_t i=0;i<other.countertags.size();i++){
            countertags.push_back(other.countertags[i]);
            tagsname.push_back(other.tagsname[i]);
        }
    }


    vector<int> User::getwatchedlist()const{
        return watchedlist;
    }

    void User::clean(){
        for(size_t i=0;i<history.size();i++){
            history.at(i)=nullptr;
        }
    }

    void User::copy(const User& other){
        while(!history.empty()){
            history.pop_back();
            watchedlist.pop_back();
        }

    }

    void User::push(Watchable *watch){
        history.push_back(watch);
//        for(size_t i=0;i<watchedlist.size();i++)
//            cout<<i<<endl;
        watchedlist.push_back(watch->getid()-1);//matched the place in content

    }


    //------------------------------ LengthRecommenderUser---------------------------------


    LengthRecommenderUser::LengthRecommenderUser(const string &name) :User(name){
    }

    LengthRecommenderUser::LengthRecommenderUser(const string &name, const User &other) : User(name,other) {//copy c'tor
    }

    LengthRecommenderUser::LengthRecommenderUser(const string &name,  User &&other) : User(name,other) {//copy c'tor
    }

    LengthRecommenderUser &LengthRecommenderUser::operator=(const LengthRecommenderUser & other) {
        if(this==&other){
            return *this;
        }
        clean();
        copy(other);
        return *this;
    }

    LengthRecommenderUser &LengthRecommenderUser::operator=( LengthRecommenderUser&& other) {

        clean();
        steal(other);
        return *this;
    }

    string LengthRecommenderUser::toString() const{
        string s="len";
        return s;
    }


    User* LengthRecommenderUser::clone(){
        User *temp=new LengthRecommenderUser(getName(),*this);
        return temp;
    }

    Watchable* LengthRecommenderUser::getRecommendation(Session& s){

        int sum =0;
        for (size_t k=0;k<this->get_history().size();k++)
        {
            sum =sum+ (get_history()[k]->getlength()) ;
        }
        int average= sum / (history).size();

        vector<Watchable*> _s=s.getContent();
        Watchable* closer=nullptr;
        if(!_s.empty()) {
            for (size_t i = 0; i < (_s).size(); i++) {
                if (!contains(&history, (_s)[i])) {
                    if (closer == nullptr) {
                        closer = (_s)[i];
                    } else if (abs((_s)[i]->getlength() - average) < abs(closer->getlength() - average)) {
                        closer = (_s)[i];
                    }
                }

            }
        }
        return closer;
    }


    //------------------------------ RerunRecommenderUser---------------------------------

    RerunRecommenderUser::RerunRecommenderUser(const string &name): User(name) ,i(0){

    }
    RerunRecommenderUser::RerunRecommenderUser(const string& name, const User& other):User(name,other),i(0){
    }
    RerunRecommenderUser::RerunRecommenderUser(const string& name,  User&& other):User(name,other),i(0){
    }

    RerunRecommenderUser &RerunRecommenderUser::operator=(const RerunRecommenderUser & other) {
        if(this==&other){
            return *this;
        }
        clean();
        copy(other);
        return *this;
    }

    RerunRecommenderUser &RerunRecommenderUser::operator=( RerunRecommenderUser&& other) {

        clean();
        steal(other);
        return *this;
    }

    User* RerunRecommenderUser::clone(){
        User *temp=new RerunRecommenderUser(getName(),*this);
        return temp;
    }

    string RerunRecommenderUser::toString() const{
        string s="rer";
        return s;
    }

    Watchable* RerunRecommenderUser::getRecommendation(Session& s){
        if(!history.empty()){
            i++;
            return history[(i)%history.size()];
        }
        return nullptr;
    }


    //------------------------------ GenreRecommenderUser---------------------------------


    GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name),famoustags(0) {
    }

    GenreRecommenderUser::GenreRecommenderUser(const string &name, const User &other) : User(name,other),famoustags(0) {
    }
    GenreRecommenderUser::GenreRecommenderUser( const GenreRecommenderUser &other) : User(other.getName(),other),famoustags(0) {
    }

    GenreRecommenderUser::GenreRecommenderUser(const string &name,  User &&other) : User(name,other),famoustags(0) {
    }

    GenreRecommenderUser &GenreRecommenderUser::operator=(const GenreRecommenderUser & other) {
        if(this==&other){
            return *this;
        }
        clean();
        copy(other);
        return *this;
    }

    GenreRecommenderUser &GenreRecommenderUser::operator=( GenreRecommenderUser&& other) {

        clean();
        steal(other);
        return *this;
    }

    User* GenreRecommenderUser::clone(){
        User *temp=new GenreRecommenderUser(getName(),*this);
        return temp;
    }

    GenreRecommenderUser::~GenreRecommenderUser(){
        clean();
        for(size_t i=0;i<famoustags.size();i++){
            delete (famoustags)[i];
            (famoustags)[i]=nullptr;
        }
    }


    string GenreRecommenderUser::toString() const{
        string s="gen";
        return s;
    }
    Watchable* GenreRecommenderUser::getRecommendation(Session& s){
        vector<string>* tags=history[history.size()-1]->retvectag();
        if(famoustags.empty()){
//            famoustags=new vector<pair<int,string>*>;
            for(auto & tag : *tags){
                famoustags.push_back(new pair<int,string>(1,tag));
            }
        }else{
            bool there;
            for(size_t i=0;i<tags->size();i++){
                there=false;
                for(size_t j=0;!there&&j<famoustags.size();j++){
                    if((*tags)[i]==(famoustags)[j]->second){
                        there=true;
                        (famoustags)[j]->first++;
                    }
                }
                if(!there){
                    famoustags.push_back(new pair<int,string>(1,(*tags)[i]));
                }
            }
        }
        Watchable* famous=nullptr;
        if(!famoustags.empty()) {
            if(famoustags.size()>1){
                insertionSort();//sorting the vector in both terms
            }
            vector<Watchable *> content = s.getContent();
            int fame, currfame;
            for (size_t i = 0; i < (content).size(); i++) {
                if (!contains(&(history), (content)[i])) {
                    currfame = tagfame((content)[i]);
                    if (famous == nullptr) {
                        famous = (content)[i];
                        fame = tagfame(famous);
                    } else if (currfame < fame) {
                        fame = currfame;
                        famous = (content)[i];
                    }
                }

            }
        }
        return famous;
    }

    int GenreRecommenderUser::tagfame(Watchable *curr){
        vector<std::string>* tags=curr->retvectag();
        int i=famoustags.size();
        for(size_t k=0;k<tags->size();k++){
            for(size_t j=0;j<famoustags.size();j++){
                if((*tags)[k]==(famoustags)[j]->second){
                    if(static_cast<int>(j)<i){
                        i=j;
                    }
                    break;
                }
            }
        }
        return i;
    }




    void GenreRecommenderUser::insertionSort(){
        pair<int,string>* temp;
        int j;
        for (size_t i = 1; i < famoustags.size(); i++)
        {
            temp = (famoustags)[i];
            j =static_cast<int>( i - 1);

            while (j >= 0 && (famoustags)[j]->first < temp->first)
            {
                (famoustags)[j + 1] = (famoustags)[j];
                j = j - 1;
            }
            while (j >= 0 && (famoustags)[j]->first == temp->first && (famoustags)[j]->second <temp->second)
            {
                (famoustags)[j + 1] = (famoustags)[j];
                j = j - 1;
            }
            (famoustags)[j + 1] = temp;

        }
    }