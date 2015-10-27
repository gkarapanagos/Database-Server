/*
Author: Georgios Karapanagos - gkarapanagos
Date: 09/12/2015

This is the server function that receives data from the client and manages the server's response.

Code taken from the "TCP/IP Sockets in C" book by Donahoo and Calvert.
The original code can be found at: http://cs.baylor.edu/~donahoo/practical/CSockets/code/TCPEchoServer.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Database.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define RCVBUFSIZE 512   /* Size of receive buffer */

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]) {

  // Server Port set fixed at 2000
  in_port_t servPort = 2003;

  char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
  int recvMsgSize;                    /* Size of received message */
  string line;
  int k = 0;
  // INITIALIZE DATABASE
  Database DB;

  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithSystemMessage("socket() failed");

  // Construct local address structure
  struct sockaddr_in servAddr;                  // Local address
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort);          // Local port

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");

  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");

  for (;;) { // Run forever
    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!

    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL){

         /* Receive message from client */
        if ((recvMsgSize = recv(clntSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithSystemMessage("recv() failed");
        line = echoBuffer;

      	if(line != ""){
              // If a command is found
              DB = HandleCommand(DB, (char*)line.c_str(), clntSock, servSock);
            }


        if(line.find("EOF")==5) 
            // Check if end of transmission
            {
            // Return appropriate message
            ofstream outputFile;
            outputFile.open("MEDatabase.txt");
            while((k < 99) && DB.recordDB[k].id_number > 0){
              outputFile << "ID: " << DB.recordDB[k].id_number
              << ", First Name: " << DB.recordDB[k].first_name
              << ", Last Name: " << DB.recordDB[k].last_name
              << ", Gender: " << DB.recordDB[k].gender
              << ", Location: " << DB.recordDB[k].location << "\n";
              k ++;
              }
            outputFile.close();
            close(servSock);
            break;
            }

            // Clear the buffer
            memset(&echoBuffer[0], 0, sizeof(echoBuffer));

            // Receive the same string back from the server
            unsigned int totalBytesRcvd = 0; // Count of total bytes received

            while (totalBytesRcvd < 1) {
                char buffer[512]; // I/O buffer
                 // Receive up to the buffer size (minus 1 to leave space for
                 // a null terminator) bytes from the sender 
                unsigned int numBytes = recv(servSock, buffer, 512 - 1, 0);
                if (numBytes < 0)
                  DieWithSystemMessage("recv() failed");
                else if (numBytes == 0)
                  DieWithUserMessage("recv()", "connection closed prematurely");

                totalBytesRcvd += numBytes; // Keep tally of total bytes
                buffer[numBytes] = '\0';    // Terminate the string!
        }
  	}
    else
      puts("Unable to get client address");
  }
}
