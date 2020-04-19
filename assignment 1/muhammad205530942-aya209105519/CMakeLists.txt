cmake_minimum_required(VERSION 3.15)
project(muhammad205530942_aya209105519)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(muhammad205530942_aya209105519
        include/Action.h
        include/json.hpp
        include/Session.h
        include/User.h
        include/Watchable.h
        src/Action.cpp
        src/Main.cpp
        src/Session.cpp
        src/User.cpp
        src/Watchable.cpp)
