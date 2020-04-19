    //
    // Created by Muhammad Bader on 12/01/2020.
    //

    #include <iostream>
    #include <protocol.h>
    #include <string>
    #include <vector>
    #include <unordered_map>
    #include <sstream>
    #include <mutex>

    using namespace std;

    protocol::protocol(ConnectionHandler* connect,std::mutex *mutex):connectionHandler(connect),mute(mutex),username(),idTopic(new unordered_map<string,string>(0)),library(new unordered_map<string,vector<string>*>(0)),Booklender(new unordered_map<string,string>(0)),receiptRequests(new unordered_map<string,string>(0)),online(false),wishToBorrow(new unordered_map<std::string,std::vector<std::string>>){
    }

    protocol::protocol(const protocol &aProtocol)  {
        copy(aProtocol);
    }

    protocol protocol::operator=(const protocol &aProtocol) {
        if(this!=&aProtocol){
            clear();
            copy(aProtocol);
        }
        return *this;
    }

    protocol::~protocol() {
        clear();
    }
    void protocol::clear(){
        delete idTopic;
        delete Booklender;
        delete receiptRequests;
        for(auto x:*library){
            delete(x.second);
        }
        delete library;
        delete wishToBorrow;
    }

    void protocol::copy(const protocol& aProtocol){
        connectionHandler=aProtocol.connectionHandler;
//        mute=aProtocol.mute;
        username=aProtocol.username;
        idTopic=new unordered_map<string,string>;
        if(!aProtocol.idTopic->empty()){
            for(auto s:*aProtocol.idTopic){
                string a=s.first;
                string b=s.second;
                idTopic->insert({a,b});
            }
        }
        library=new unordered_map<string,vector<string>*>;
        if(!aProtocol.library->empty()){
            for(auto s:*aProtocol.library){
                string a=s.first;
                vector<string>* vec=new vector<string>;
                for(string q:*s.second){
                    string b=q;
                    vec->push_back(b);
                }
                library->insert({a,vec});
            }
        }
        Booklender=new unordered_map<string,string>;
        if(!aProtocol.Booklender->empty()){
            for(auto s:*aProtocol.Booklender){
                string a=s.first;
                string b=s.second;
                Booklender->insert({a,b});
            }
        }

        receiptRequests=new unordered_map<string,string>;
        if(!aProtocol.receiptRequests->empty()){
            for(auto s:*aProtocol.receiptRequests){
                string a=s.first;
                string b=s.second;
                receiptRequests->insert({a,b});
            }
        }
        online=aProtocol.online;

        wishToBorrow=new unordered_map<string,vector<string>>;
        if(!aProtocol.wishToBorrow->empty()){
            for(auto s:*aProtocol.wishToBorrow){
                string a=s.first;
                vector<string> vec;
                for(string q:s.second){
                    string b=q;
                    vec.push_back(b);
                }
                wishToBorrow->insert({a,vec});
            }
        }
    }

    bool protocol::login(string message,ConnectionHandler& connectionHandler){//check aya
        vector<string> parse=readingInput(message,' ');

        if(parse[0]!="login") return false;
        string frame="CONNECT\naccept-version:1.2\nhost:"+parse[1]+"\nlogin:"+parse[2]+"\npasscode:"+parse[3]+"\n\n";//create the CONNECT frame
        connectionHandler.sendLine(frame);
        string answer;//receive the answer frame
        connectionHandler.getLine(answer);
        vector<string> ans=readingInput(answer,'\n');
        if(ans[0]=="CONNECTED"){
            username=parse[2];
            //start the communication with the server, which means deal with the threads
            std::cout<<"Login successful"<<std::endl;
            online=true;
            return true;
        }else if(ans[0]=="ERROR"){
            cout<<ans[ans.size()-1]<<endl;//print the error
            return false;
        }
        return false;
    }

    void protocol::doTheJob(string answer) {
        vector<string> frames=readingInput(answer,'\n');
        istringstream parsing(answer);
        string body;
        if (frames[0] == "ERROR") {//ready

            cout << frames[0] << endl;
            cout << frames[frames.size()-2] << endl;
            cout << frames[frames.size()-1] << endl;

            // and disconnect from the socket// DONE
            online = false;
        } else if (frames[0] == "RECEIPT") {

            string receiptID=readingInput(frames[1],':')[1];

            string toDo = getReceipt(receiptID);
            vector<string> command=readingInput(toDo,' ');

            if (command[0] == "joined") {
                string id=command[1];
                string club=command[2];

                JoinClub(id, club);
                cout << "Joined club " << club << endl;
            } else if (command[0] == "exitclub") {
                string club=command[1];
                ExitClub(club);
                cout << "Exited club " << club << endl;
            } else if (command[0] == "logout") {
                online = false;
            }
        } else if (frames[0] == "MESSAGE") {
            vector<string> bodyParts = readingInput(frames[4], ' ');
            //print the body of the answer
            cout << frames[4] << endl;

            if (bodyParts[0] == username) {
                if (bodyParts[2] ==
                    "added") {//if the answer contain "username has added the book" add the book to the library..DONE
                    string book;
                    for (size_t i = 5; i < bodyParts.size() - 1; i++) {
                        book.append(bodyParts[i] + " ");
                    }
                    book.append(bodyParts[bodyParts.size() - 1]);
                    string topic = (readingInput(frames[3], ':'))[1];
                    addBook(book, topic);

                } else if (bodyParts[3] == "borrow") {//I wish to borrow.. DONE
                    size_t i = 4;
                    string book;
                    while (i < bodyParts.size() - 1) {
                        book.append(bodyParts[i] + " ");
                    }
                    book.append(bodyParts[bodyParts.size() - 1]);
                    string topic = (readingInput(frames[3], ':'))[1];
                    wantToBorrow(topic, book);
                    //this message means you wish to borrow a book
                } else if (bodyParts[2] !=
                           "added") {//continue, you have the book that someone wishes to borrow..DONE
                    //do nothing
                }
            } else if (frames[4] ==
                       "Book status") {//if the body is "book status" send your library as a SEND frame ..DONE
                string message = getstatus(readingInput(frames[3], ':')[1]);
                connectionHandler->sendLine(message);
            } else if (bodyParts[0] ==
                       "Taking") {//three cases, you are taking or someone taking from you or you are neutral
                string lender = bodyParts[bodyParts.size() - 1];
                if (lender == username) {//taking from me
                    string topic = (readingInput(frames[3], ':'))[1];
                    string book;
                    size_t i = 1;
                    while (i < bodyParts.size() - 3) {
                        book.append(bodyParts[i] + " ");
                        i++;
                    }
                    book.append(bodyParts[bodyParts.size() - 3]);
                    deleteBook(book, topic);//someone has borrowed the book
                }
            } else if (bodyParts[0] == "Returning") {//DONE
                string lender = bodyParts[bodyParts.size() - 1];
                if (lender == username) {//if it is not me don't do anything
                    string topic = (readingInput(frames[3], ':'))[1];
                    string book;
                    size_t i = 1;
                    while (i < bodyParts.size() - 3) {
                        book.append(bodyParts[i] + " ");
                        i++;
                    }
                    book.append(bodyParts[bodyParts.size() - 3]);
                    addBook(book, topic);
                }
            } else if (bodyParts.size()>1) {//someone wish to borrow, so check if you have this book..DONE
                if(bodyParts[1] == "wish"){
                    size_t i = 4;
                    string book;
                    while (i < bodyParts.size() - 1) {
                        book.append(bodyParts[i] + " ");
                    }
                    book.append(bodyParts[bodyParts.size() - 1]);
                    string topic = (readingInput(frames[3], ':'))[1];
                    if (hasBook(book, topic)) {//if I have the book then send a message
                        string msg = "SEND\ndestination:" + topic + "\n\n" + username + " has " + book + "\n";
                        connectionHandler->sendLine(msg);
                    }
                }else if (bodyParts[1] == "has") {//someone has a book,,,check if you want to borrow it
                    size_t i = 2;
                    string book;
                    string lender = bodyParts[0];
                    while (i < bodyParts.size() - 1) {
                        book.append(bodyParts[i] + " ");
                    }
                    book.append(bodyParts[bodyParts.size() - 1]);
                    string topic = (readingInput(frames[3], ':'))[1];
                    if (IwantToBorrow(topic, book)) {
                        //send a SEND frame:: Taking... DONE
                        borrowBook(book, topic, lender);
                        string send = "SEND\ndestination:" + topic + "\n\n" + "Taking " + book + " from " + lender + "\n";
                        connectionHandler->sendLine(send);
                    }
                }

            }
        }
    }

    vector<string> protocol::readingInput(string& s,char x){
        string w;
        vector<string> input;
        istringstream parsing(s);
        while(getline(parsing,w,x)){
            if(w!=""){
                input.push_back(w);
            }
        }
        return input;
    }

    bool protocol::hasBook(string book, string topic) {//ready
        vector<string>* shelf=library->at(topic);
        for(auto x:*shelf){
            if(x==book)
                return true;
        }
        return false;
    }

    void protocol::addBook(string book,string topic) {//ready
        mute->lock();
        unordered_map<string,vector<string>*>::iterator isThere=library->find(topic);
        if(isThere==library->end()){//new topic
            //we chose to create the shelf vector here and not in JoinClub method
            vector<string> *vec=new vector<string>;
            vec->push_back(book);
            library->insert({topic,vec});
        }else{//already registered
            vector<string>* vec=library->at(topic);
            vec->push_back(book);
        }
        mute->unlock();
    }

    void protocol::borrowBook(string book, string topic,string lender) {//ready

        Booklender->insert({book,lender});
        for(auto id:*idTopic){
            if(id.second==topic){//which means he really joined..should always be true
                auto isThere=idTopic->find(topic);
                if(isThere!=idTopic->end()){//he already have a shelf
                    vector<string>* shelf=(*library).at(topic);
                    shelf->push_back(book);
                }else{//this is the first book to add to his shelf
                    vector<string> *vec=new vector<string>;
                    vec->push_back(book);
                    library->insert({topic,vec});
                }
            }
        }
    }

    void protocol::deleteBook(string book,string topic){//ready
        mute->lock();
        auto isThere=library->find(topic);
        int i=0;
        if(isThere!=library->end()){//check if the client has added any book to the shelf
            vector<string>* shelf=((library)->at(topic));
            for(auto shelfbook:*shelf){
                if(book==shelfbook){
                    shelf->erase(shelf->begin()+i);//remove the book from the library
                    return;
                }else{
                    i++;
                }
            }
        }
        mute->unlock();
    }

    void protocol::JoinClub(string subscriptionID,string topic) {//ready
        for(auto x:*idTopic){
            if(x.second==topic){
                // this shouldn't happen
                // that means the Client registered to the same topic more than once
                return;
            }
            idTopic->insert({subscriptionID,topic});
            //we will deal with the library here
            vector<string> *shelf=new vector<string>;
            library->insert({topic,shelf});
        }
    }

    void protocol::ExitClub(std::string topic) {//ready
        for (auto id:*idTopic) {
            if (id.second == topic) {//check if the Client subscribed for this topic
                idTopic->erase(id.first);//erase the subscription
                auto f=(*library).find(topic);
                if(f!=(*library).end()){//if there is a shelf for this topic
                    vector<string> *shelf = (*library).at(topic);
                    for (auto book:*shelf) {//check if the book was borrowed from someone
                        auto lendedBook = Booklender->find(book);//run through all the books and check if 'book'is there
                        if (lendedBook != (*Booklender).end()) {//if it is really there delete it
                            (*Booklender).erase(book);
                        }//done with the lender
                    }
                    delete shelf;
                }
                library->erase(topic);
                break;
            }
        }
    }

    string protocol::getstatus(string topic) {//ready
        auto isThere=library->find(topic);
        if(isThere!=library->end()){//we have a shelf (books) for this topic
            vector<string>* shelf=library->at(topic);
            if(!(shelf->empty())){//we really have books on the shelf
                //create a asnwer with destination:topic and username:all the books with commas..DONE
                string body=username+":"+(*shelf)[0];
                for(size_t i=1;i<(*shelf).size();i++){
                    body.append(","+(*shelf)[i]);
                }
                string message="SEND\ndestination:"+topic+"\n\n"+body+"\n";
                return message;
            }else{//an empty shelf
                string message="SEND\ndestination:"+topic+"\n\n"+username+":\n";
                // create a asnwer with destination:topic and username: ..DONE
                // just return topic \n username ..DONE
                return message;
            }
        }else{//no book in this topic
            // create a asnwer with destination:topic and username: ..DONE
            // just return topic \n username ..DONE
            string message="SEND\ndestination:"+topic+"\n\n"+username+":\n";
            return message;
        }
    }

    //todo check aya
    void protocol::addReceipt(std::string receiptID,std::string receiptWork) {
        receiptRequests->insert({receiptID, receiptWork});
    }

    string protocol::getReceipt(std::string id) {
        return receiptRequests->at(id);
    }

    bool protocol::isOnline() {return online;}

    void protocol::wantToBorrow(std::string topic, std::string book) {
        auto there=wishToBorrow->find(topic);
        if(there!=wishToBorrow->end()){//there is a victor already
            wishToBorrow->at(topic).push_back(book);
        }else{//new topic to borrow from
            vector<string> tmp;
            tmp.push_back(book);
            wishToBorrow->insert({topic,tmp});
        }
    }

    bool protocol::IwantToBorrow(std::string topic, std::string book) {
        auto f=wishToBorrow->find(topic);
        if(f!=wishToBorrow->end()){
            for(size_t i=0;i<wishToBorrow->at(topic).size();i++){
                if(book==wishToBorrow->at(topic)[i]){
                    wishToBorrow->at(topic).erase(wishToBorrow->at(topic).begin()+i);
                    return true;
                }
            }
        }
        return false;
    }

    std::string protocol::getIDtopic(std::string topic) {
        for(auto x:*idTopic){
            if(x.second==topic){
                return x.first;
            }
        }
        return "";
    }

    std::string protocol::getname() {
        return username;
    }

    std::string protocol::getlender(std::string book) {
        string lender=Booklender->at(book);
        return lender;
    }

    void protocol::returnbook(std::string book) {
        Booklender->erase(book);
    }

