/*
Author: Georgios Karapanagos - gkarapanagos
Date: 09/12/2015

This is the basic client that sends data to the ME server.

Code taken from the "TCP/IP Sockets in C" book by Donahoo and Calvert.
The original code can be found at: http://cs.baylor.edu/~donahoo/practical/CSockets/code/TCPEchoClient.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Database.h"
#include <string>
#include <netdb.h>
#include <iostream>
#include <fstream>
using namespace std;
struct hostent *hp;

int main(int argc, char *argv[]) {

  // Server Port set fixed at 2000
  in_port_t servPort = 2003;

  // Get Server IP Addres
    if((hp=gethostbyname(argv[1]))==NULL){
          printf("can not get the host by this wat");
          exit(-1);
  }


  char *fileName;
  ifstream in;
  // Determine whether input from text file
  // or from a single command line
  bool textFile = false;
  if(argc == 3) {
    char * check = strstr(argv[2], ".txt");
    // READ ONE LINE FROM THE TEXT
    // SEND THE ONE LINE COMMAND TO
    // THE SERVER TO HANDLE IT
    if(check != NULL){
      fileName = argv[2];
      in.open(fileName);
      textFile = true;
    }
  }

  // String holding the command line input
  string input;
  char* line;

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  bcopy(hp->h_addr,(char *)&servAddr.sin_addr,hp->h_length);

  
  if(textFile) {
    while(getline(in,input)){

    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
      DieWithSystemMessage("socket() failed");


    // Convert address
    int rtnVal = inet_pton(AF_INET, inet_ntoa(servAddr.sin_addr), &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
      DieWithUserMessage("inet_pton() failed", "invalid address string");
    else if (rtnVal < 0)
      DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);    // Server port



    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
      DieWithSystemMessage("connect() failed");

    

          line = (char*)input.c_str();

          // Determine input length
          unsigned int lineLen = strlen(line); 

          // Send the string to the server
          unsigned int numBytes = send(sock, line, lineLen, 0);

          // Receive the same string back from the server
          unsigned int totalBytesRcvd = 0; // Count of total bytes received


          while (totalBytesRcvd < 512) {
            char buffer[512]; // I/O buffer
             // Receive up to the buffer size (minus 1 to leave space for
             // a null terminator) bytes from the sender 
            numBytes = recv(sock, buffer, 512 - 1, 0);
            if (numBytes < 0)
              DieWithSystemMessage("recv() failed");
            else if (numBytes == 0)
              DieWithUserMessage("recv()", "connection closed prematurely");

            totalBytesRcvd += numBytes; // Keep tally of total bytes
            buffer[numBytes] = '\0';    // Terminate the string!
            fputs(buffer, stdout);      // Print the echo buffer
          }

          fputc('\n', stdout); // Print a final linefeed
          send(sock, "d", 1, 0);

          if(input.find("EOF")==5) break;

          close(sock);

    }

    in.close();

    }



    int k;
    if(!textFile) {
      input = "";
      for(k = 2; k < argc; k++)
        { input += argv[k];
          input += " ";
        }

    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
      DieWithSystemMessage("socket() failed");


    // Convert address
    int rtnVal = inet_pton(AF_INET, inet_ntoa(servAddr.sin_addr), &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
      DieWithUserMessage("inet_pton() failed", "invalid address string");
    else if (rtnVal < 0)
      DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);    // Server port



    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
      DieWithSystemMessage("connect() failed");

          

          line = (char*)input.c_str();

          // Determine input length
          unsigned int lineLen = strlen(line); 

          // Send the string to the server
          unsigned int numBytes = send(sock, line, lineLen, 0);

          // Receive the same string back from the server
          unsigned int totalBytesRcvd = 0; // Count of total bytes received


          while (totalBytesRcvd < 512) {
            char buffer[512]; // I/O buffer
             // Receive up to the buffer size (minus 1 to leave space for
             // a null terminator) bytes from the sender 
            numBytes = recv(sock, buffer, 512 - 1, 0);
            if (numBytes < 0)
              DieWithSystemMessage("recv() failed");
            else if (numBytes == 0)
              DieWithUserMessage("recv()", "connection closed prematurely");

            totalBytesRcvd += numBytes; // Keep tally of total bytes
            buffer[numBytes] = '\0';    // Terminate the string!
            fputs(buffer, stdout);      // Print the echo buffer

          }
           cout << "\n";
          send(sock, " ", 1, 0);

          close(sock);
    }

  
  exit(0);
}
