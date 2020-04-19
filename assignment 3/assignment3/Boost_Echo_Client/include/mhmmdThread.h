//
// Created by Muhammad Bader on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MHMMDTHREAD_H
#define BOOST_ECHO_CLIENT_MHMMDTHREAD_H

#include "protocol.h"

class mhmmdThread{
private:
    protocol* proto;
    ConnectionHandler* connect;
public:
    mhmmdThread(protocol* proto,ConnectionHandler* connect);
    void doTheJob();

};
#endif //BOOST_ECHO_CLIENT_MHMMDTHREAD_H
