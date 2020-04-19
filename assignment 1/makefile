# All Targets
all: start
# Tool invocations
# Executable "debug" depends on the files hello.o and run.o.
start: bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/Action.o
		g++ -o bin/start bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/Action.o
       

# Depends on the source and header files
bin/Main.o: src/Main.cpp
		g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Session.o: src/Session.cpp
		g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

# Depends on the source and header files
bin/Watchable.o: src/Watchable.cpp
		g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

# Depends on the source and header files 
bin/User.o: src/User.cpp
		g++ -g -Wall -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

# Depends on the source and header files 
bin/Action.o: src/Action.cpp
		g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

#Clean the build directory
clean: 
		rm -f bin/*