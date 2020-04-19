CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: STOMPClient
	g++ -o bin/STOMPClient bin/connectionHandler.o bin/echoClient.o bin/protocol.o bin/ayaThread.o bin/mhmmdThread.o $(LDFLAGS)

STOMPClient: bin/connectionHandler.o bin/echoClient.o bin/protocol.o bin/ayaThread.o bin/mhmmdThread.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/protocol.o: src/protocol.cpp
	g++ $(CFLAGS) -o bin/protocol.o src/protocol.cpp

bin/ayaThread.o: src/ayaThread.cpp
	g++ $(CFLAGS) -o bin/ayaThread.o src/ayaThread.cpp

bin/mhmmdThread.o: src/mhmmdThread.cpp
	g++ $(CFLAGS) -o bin/mhmmdThread.o src/mhmmdThread.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
