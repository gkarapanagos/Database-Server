# Author : Georgios Karapanagos - gkarapanagos
# Date : 09/12/2015
# make MEClient -> create the ME Client
# make MEServer -> create the ME Server

CFLAGS = -g

all: MEClient MEServer

MEClient: TCPClient.o Database.o HandleCommand.o DieWithMessage.o Database.h
	g++ $(CPPFLAGS) -m32 -Wall -lm Database.o HandleCommand.o TCPClient.o DieWithMessage.o  -o MEClient

MEServer: TCPServer.o Database.o HandleCommand.o DieWithMessage.o Database.h
	g++ $(CPPFLAGS) -m32 -Wall -lm TCPServer.o Database.o HandleCommand.o DieWithMessage.o  -o MEServer

TCPServer.o:  TCPServer.cpp Database.h
	g++ $(CPPFLAGS) -m32 -Wall -c TCPServer.cpp

TCPClient.o: TCPClient.cpp DieWithMessage.o Database.h
	g++ $(CPPFLAGS) -m32 -Wall DieWithMessage.o -c TCPClient.cpp

DieWithMessage.o: DieWithMessage.cpp 
	g++ $(CPPFLAGS) -m32 -Wall -c DieWithMessage.cpp

HandleCommand.o: HandleCommand.cpp Database.o Database.h
	g++ $(CPPFLAGS) -m32 -Wall -c HandleCommand.cpp
	
Database.o: Database.cpp Database.h
	g++ $(CPPFLAGS) -m32 -Wall -c Database.cpp

clean:
	rm -f TCPClient.o MEClient MEServer.o MEServer MEClient.o Database.o DieWithMessage.o TCPServer.o HandleCommand.o
