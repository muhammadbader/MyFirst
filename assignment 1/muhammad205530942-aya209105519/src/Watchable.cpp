    #include "../include/Watchable.h"
    #include "Session.h"
    #include <string>
    #include <vector>
    using namespace std;



    Watchable::Watchable(long id_, int length_, const std::vector<std::string>& tags_): id(id_),length(length_),tags(tags_){
    }

    Watchable::Watchable(const Watchable& other):id(other.id),length(other.length),tags(vector<string>(0)){
        for(size_t i=0;i<other.tags.size();i++){
            tags.push_back(other.tags[i]);
        }
    }

    Watchable:: ~Watchable(){
    }


    int Watchable::getlength() const {
        return length;
    }

    long Watchable::getid() const {
        return id;
    }

    string Watchable::getkind() const {
        return "Watchable";
    }

    std::vector<std::string>* Watchable::retvectag() {
        return &tags;
    }

    void Watchable::setnextid(long next) {

    }


    std::string Watchable::gettags() const {
        string s="[";
        for(size_t i=0;i < tags.size()-1;i++){
            s.append(tags[i]);
            s.append(", ");
        }
        s.append(tags[tags.size()-1]);
        s.append("]");
        return s;
    }


    void Watchable::setLength(int len){
        length=len;
    }
    void Watchable::settag(string tag){
        tags.push_back(tag);
    }
    int Watchable::lentag() const{
        return tags.size();
    }
    string Watchable::tag(int i)const{
        return tags[i];
    }
    int Watchable::getseason() const{
        return 0;
    }
    int Watchable::getepisode() const{
        return 0;
    }

    //--------------------------------------------------------- Movie-----------------------------------------------------------

    Movie::Movie(long id_, const std::string& name_, int length_, const std::vector<std::string>& tags_):Watchable(id_,length_,tags_),name(name_){

    }

    Movie::Movie(string name,const Watchable& other):Watchable(other),name(name){
    }
    Movie::Movie(const Movie& other):Watchable(other),name(other.name){
    }

    Movie &Movie::operator= (const Movie& other ){
        
        if (this==&other)
        {
            return *this;
        }
        setLength(other.getlength());

        while(!retvectag()->empty()){
            retvectag()->pop_back();
        }

        for(int i=0;i<other.lentag();i++){
            settag((other.tag(i)));
        }

        return *this;
    }
    Watchable* Movie::getNextWatchable( Session & other) const{
        Watchable *temp=other.getactive()->getRecommendation(other);
        return temp;
    }

    Watchable* Movie::clone(){
        Movie *temp=new Movie(getid(),getname(),getlength(),*retvectag());
        return temp;
    }

    string Movie::getkind() const {
        string s="Movie";
        return s;
    }

    long Movie::getnextIDepisode() const{
        return 0;
    }

    std::string Movie::getname() const{
        return this->name;
    }

    std::string Movie::toString() const{
        string s="";
        s.append(this->getname());

        return s;
    }

    std::string Movie::toString2() const{
        string s="";
        s.append(this->getname());
        s.append(" ");
        s.append(to_string(this->getlength()));
        s.append(" minutes ");
        s.append(this->gettags());

        return s;
    }
    //--------------------------------------------------------------------Episode-------------------------------------------------------------

    Watchable* Episode::getNextWatchable(Session & sess) const {
        Watchable *last=(sess.getactive()->get_history()[sess.getactive()->get_history().size()-1]);//the last watched content
    //check the next episode if available!!!!

        if(last->getnextIDepisode()!=0){
            // return: search for the next episode
            last=(sess.getContent())[last->getnextIDepisode()-2];
        }else{
            last=sess.getactive()->getRecommendation(sess);
        }
        return last;

    }


    Episode::Episode(const std::string& seriesName_, int season_, int episode_ ,const Watchable& other,long nextid):Watchable(other),seriesName(seriesName_),season(season_),episode(episode_),nextEpisodeId(nextid){

    }
    Episode::Episode(const Episode &other) :Watchable(other),seriesName(other.seriesName),season(other.season),episode(other.episode),nextEpisodeId(other.nextEpisodeId){

    }
    Episode::Episode(long id, const std::string& seriesName_,int length, int season_, int episode_ ,const std::vector<std::string>& tags):Watchable(id,length,tags),seriesName(seriesName_),season(season_),episode(episode_),nextEpisodeId(0){//copy c'tor

    }


    Episode &Episode::operator= (const Episode& other ){
        if (this==&other)
        {
            return *this;
        }
        seriesName=other.seriesName;
        season=other.season;
        episode=other.episode;
        nextEpisodeId=other.nextEpisodeId;
        while(!retvectag()->empty()){
            retvectag()->pop_back();
        }

        for(int i=0;i<other.lentag();i++){
            settag((other.tag(i)));
        }

        return *this;
    }
    Watchable* Episode::clone(){
        Episode *temp= new Episode(getid(),getname(),getlength(),getseason(),getepisode(),*retvectag());
        return temp;
    }

    int Episode::getseason() const{
        return this->season;
    }
    int Episode::getepisode() const{
        return this->episode;
    }



    string Episode::getkind() const {
        string s="Episode";
        return s;
    }

    string Episode::getname() const{
        return this->seriesName;
    }

    std::string Episode::toString() const{
        string s="";
        s.append(getname());
        s.append(" ");
        s.append(getse());


    return s;
    }
    std::string Episode::toString2() const{
        string s="";
        s.append(getname());
        s.append(" ");
        s.append(getse());
        s.append(" ");
        s.append(to_string(getlength()));
        s.append(" minutes ");
        s.append(this->gettags());
        return s;

    }


    std::string Episode::getse()const {
        string s("");
        s.append("S");
        s.append(to_string(season));
        s.append("E");
        s.append(to_string(episode));
        return s;

    }
    long Episode::getnextIDepisode() const{
        return nextEpisodeId;
    }
    void Episode::setnextid(long next) {
        nextEpisodeId=next;
    }

