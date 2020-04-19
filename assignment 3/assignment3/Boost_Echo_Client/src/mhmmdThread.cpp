//
// Created by Muhammad Bader on 13/01/2020.
//

#include <mhmmdThread.h>
#include <iostream>

using namespace std;

mhmmdThread::mhmmdThread(protocol* proto,ConnectionHandler* connect):proto(proto),connect(connect){
}

void mhmmdThread::doTheJob() {
    while(proto->isOnline()){
        string answer;
        connect->getLine(answer);
        //make it a frame
        proto->doTheJob(answer);
    }
}