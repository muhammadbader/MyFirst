#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    Watchable(const Watchable&);
    virtual ~Watchable();// rule of 5!!!
    virtual std::string toString() const = 0;
    virtual std::string toString2() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    void steal(Watchable &other);
    std::string gettags() const;
    int getlength() const;
    virtual std::string getname() const=0;
    virtual long getid() const;
    virtual std::string getkind() const;
    virtual void setnextid(long);
    virtual long getnextIDepisode() const=0;
    virtual std::vector<std::string>* retvectag();
    virtual Watchable* clone()=0;
    virtual void setLength(int);
    virtual void settag(std::string);
    virtual int lentag() const;
    virtual std::string tag(int) const;
    virtual int getseason() const ;
    virtual int getepisode() const;

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(std::string name,const Watchable&);
    Movie(const Movie&);
    virtual Movie& operator= (const Movie& other);
    virtual std::string toString() const;
    virtual std::string toString2() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual long getnextIDepisode() const;
//    virtual long getid() const;
    virtual std::string getname() const;
    virtual std::string getkind() const;
    virtual Watchable* clone();

private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const std::string& seriesName_, int season_, int episode_ ,const Watchable& ,long nextid);//copy c'tor
    Episode(const Episode& );
    virtual Episode& operator= (const Episode& other);
    virtual std::string toString() const;
    virtual std::string toString2() const ;
    virtual Watchable* getNextWatchable(Session&) const;
    void setnextid(long);
    std::string getse() const;
    virtual long getnextIDepisode() const;
    virtual std::string getname() const;
    virtual int getseason() const ;
    virtual int getepisode() const;
    virtual std::string getkind() const;
    virtual Watchable* clone();


private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
