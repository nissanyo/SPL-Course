CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_thread -lboost_system

all: BBclient
	g++ -o bin/BBclient bin/BBclient.o bin/ConnectionHandler.o  $(LDFLAGS)

BBclient: bin/ConnectionHandler.o bin/BBclient.o
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/BBclient.o: src/BBclient.cpp
	g++ $(CFLAGS) -o bin/BBclient.o  src/BBclient.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
