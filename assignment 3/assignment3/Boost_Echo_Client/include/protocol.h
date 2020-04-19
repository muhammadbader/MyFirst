//
// Created by Muhammad Bader on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_PROTOCOL_H
#define BOOST_ECHO_CLIENT_PROTOCOL_H
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "connectionHandler.h"

class protocol {
private:
    ConnectionHandler* connectionHandler;
    std::mutex *mute;
    std::string username;
    std::unordered_map<std::string,std::string> *idTopic;
    std::unordered_map<std::string,std::vector<std::string>*> *library;
    std::unordered_map<std::string,std::string> *Booklender;
    std::unordered_map<std::string,std::string> *receiptRequests;
    bool online;
    std::unordered_map<std::string,std::vector<std::string>>* wishToBorrow;

public:
    protocol(ConnectionHandler* connect,std::mutex *mutex);
    protocol(const protocol &aProtocol);
    void copy(const protocol&);
    protocol operator=(const protocol&);
    virtual ~protocol();
    void clear();
    bool login(std::string message,ConnectionHandler& connectionHandler);
    void doTheJob(std::string answer);
    void addReceipt(std::string receiptID,std::string receipt);
    std::string getReceipt(std::string);
    void ExitClub(std::string topic);
    void JoinClub(std::string subscriptionID,std::string topic);
    void addBook(std::string book,std::string topic);
    void deleteBook(std::string book,std::string topic);
    bool hasBook(std::string book,std::string topic);
    void borrowBook(std::string book,std::string topic,std::string lender);
    std::string getstatus(std::string topic);
    std::vector<std::string> readingInput(std::string& s,char x);
    bool isOnline();
    bool IwantToBorrow(std::string topic,std::string book);
    void wantToBorrow(std::string topic,std::string book);
    std::string getIDtopic(std::string topic);
    std::string getname();
    std::string getlender(std::string book);
    void returnbook(std::string book);
};


#endif //BOOST_ECHO_CLIENT_PROTOCOL_H
