//
// Created by Muhammad Bader on 13/01/2020.
//
#include <string>
#include <ayaThread.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

ayaThread::ayaThread(protocol* proto,ConnectionHandler *connection,string& com):proto(proto),online(true),connection(connection),command(com) {
}
void ayaThread::process() {
    while (online) {


        vector<string> commandVec=proto->readingInput(command,' ');
//        int space = command.find_first_of(' ');
// find the first space to know the command of the message
        //relevant in getting a receiptID and subscriptionID

        if (commandVec[0] == "join") {//command login
            srand (time(NULL));
            int receiptID= rand() % 10000 + 1;
            int genreID= rand() % 10000 + 1;
//            command = command.substr(space+1);
//            int firstSpace = command.find_first_of(' ');
            string genre =commandVec[1];
            std::string frame("SUBSCRIBE\ndestination:"+genre+"\nid:"+to_string(genreID)+"\nreceipt:"+to_string(receiptID)+"\n\n");
            connection->sendLine(frame);
            string receipt="joined "+to_string(genreID)+" "+genre;
            proto->addReceipt(to_string(receiptID),receipt);
        }
        else if (commandVec[0] == "exit") {//command exit
            srand (time(NULL));
            int receiptID= rand() % 10000 + 1;
//            command = command.substr(5);
//            int firstSpace = command.find_first_of(' ');
            string genre = commandVec[1];
            string idTopic=proto->getIDtopic(genre);
            if(idTopic.size()!=0){
                string frame("UNSUBSCRIBE\nid:"+idTopic+"\nreceipt:"+to_string(receiptID)+"\n");
                connection->sendLine(frame);
                string receipt("exitclub "+genre);
                proto->addReceipt(to_string(receiptID),receipt);
            }
        } else if (commandVec[0] == "add") {//command login
//            command = command.substr(4);
//            int firstSpace = command.find_first_of(' ');
            string genre = commandVec[1];
            string book_name;
            for(size_t i=2;i<commandVec.size()-1;i++){
                book_name.append(commandVec[i]+" ");
            }
            book_name.append(commandVec[commandVec.size()-1]);
            string send("SEND\ndestination:"+genre+"\n\n"+proto->getname()+" has added the book "+book_name);
            connection->sendLine(send);
        } else if (commandVec[0] == "borrow") {//command login
//            command = command.substr(7);
//            int firstSpace = command.find_first_of(' ');
            string genre = commandVec[1];
            string book_name;
            for(size_t i=2;i<commandVec.size()-1;i++){
                book_name.append(commandVec[i]+" ");
            }
            book_name.append(commandVec[commandVec.size()-1]);
            string send("SEND\ndestination:"+genre+"\n\n"+proto->getname()+" wish to borrow "+book_name);
            connection->sendLine(send);
        } else if (commandVec[0] == "return") {//command login
//            command = command.substr(7);
//            int firstSpace = command.find_first_of(' ');
            string genre = commandVec[1];
            string book_name;
            for(size_t i=2;i<commandVec.size()-1;i++){
                book_name.append(commandVec[i]+" ");
            }
            book_name.append(commandVec[commandVec.size()-1]);
            string lender=proto->getlender(book_name);
            string send("SEND\ndestination:"+genre+"\n\n"+"Returning "+book_name+" to "+lender);
            proto->deleteBook(book_name,genre);
            proto->returnbook(book_name);
            connection->sendLine(send);
        } else if (commandVec[0] == "status") {//command login
//            command = command.substr(7);
//            int firstSpace = command.find_first_of(' ');
            string genre="";
            if(commandVec.size()==2){
                genre = commandVec[1];
            }
            string send("SEND\ndestination:"+genre+"\n\n"+"book status");
            connection->sendLine(send);
        } else if (commandVec[0] == "logout") {//command login
            srand (time(NULL));
            int receiptID= rand() % 10000 + 1;
            string receipt("logout");
            proto->addReceipt(to_string(receiptID),receipt);
//            logout(command);
        }
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        command=buf;
    }
}