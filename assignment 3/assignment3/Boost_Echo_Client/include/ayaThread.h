//
// Created by Muhammad Bader on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_AYATHREAD_H
#define BOOST_ECHO_CLIENT_AYATHREAD_H
#include "protocol.h"

class ayaThread{
private:
    protocol* proto;
    bool online;
    ConnectionHandler* connection;
    std::string &command;
public:
    ayaThread(protocol* proto,ConnectionHandler* connection,std::string& com);
    void process();

};
#endif //BOOST_ECHO_CLIENT_AYATHREAD_H
