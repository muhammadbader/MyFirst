#include <stdlib.h>
#include <connectionHandler.h>
#include <protocol.h>
#include <thread>
#include <sstream>
#include <mhmmdThread.h>
#include <ayaThread.h>
#include <mutex>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {



//todo login

    const short bufsize = 1024;
    char buf[bufsize];
    protocol *clientprotocol;
    //receiving the login command from the client
    std::cin.getline(buf, bufsize);
    std::string command(buf);
    std::mutex mute;
//     while (true){
        ConnectionHandler *connectionHandler;
        bool conn = false;
        do {

            std::string w;
            std::vector<std::string> inputIP;
            std::istringstream parsing(command);
            while (getline(parsing, w, ' ')) {
                if (w != "") {
                    inputIP.push_back(w);
                }
            }
            if(command.size()!=0){
                if(inputIP[0]=="login"){
                    if(inputIP.size()>2){
                        std::istringstream parsing2(inputIP[1]);
                        std::vector<std::string> IP;
                        getline(parsing2, w, ':');
                        IP.push_back(w);//host ip
                        getline(parsing2, w, ':');
                        IP.push_back(w);//port

                        std::string host = IP[0];
                        short port = std::atoi(IP[1].c_str());

                        connectionHandler = new ConnectionHandler(host, port);

                        if (!connectionHandler->connect()) {
                            std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                        } else {
                            //success in connecting to the server
                            clientprotocol = new protocol(connectionHandler, &mute);//initiate the reader from the server
                            if (!(clientprotocol->login(command,
                                                        *connectionHandler))) {//this line decides weather we can connect or not
                                delete clientprotocol;
                                delete connectionHandler;
                                std::cin.getline(buf, bufsize);
                                command = buf;
                            } else {
                                conn = true;
                            }
                        }
                    }else{
                        std::cin.getline(buf, bufsize);
                        command = buf;
                    }
                }else{
                    std::cin.getline(buf, bufsize);
                    command = buf;
                }
            }else{
                std::cin.getline(buf, bufsize);
                command = buf;
            }

        } while (!conn);

        //sucess in logging in

        mhmmdThread reader(clientprotocol, connectionHandler);
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        command = buf;
        ayaThread writer(clientprotocol, connectionHandler, command);

        //initiate the writer ayaThread to the server..
        std::thread th2(&ayaThread::process, &writer);
        std::thread th1(&mhmmdThread::doTheJob, &reader);
        th2.detach();
        th1.join();


        if (!(clientprotocol->isOnline())) {
            std::cout << "logout" << std::endl;
            delete clientprotocol;
            delete connectionHandler;
        }
//    }
    return 0;
}
