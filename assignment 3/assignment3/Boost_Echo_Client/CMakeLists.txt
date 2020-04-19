cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Boost_Echo_Client
        include/ayaThread.h
        include/connectionHandler.h
        include/mhmmdThread.h
        include/protocol.h
        src/ayaThread.cpp
        src/connectionHandler.cpp
        src/echoClient.cpp
        src/mhmmdThread.cpp
        src/protocol.cpp)
