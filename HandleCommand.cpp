/*
Author: Georgios Karapanagos - gkarapanagos
Date: 09/12/2015

This function handles the commands received from the server.

Code taken from the "TCP/IP Sockets in C" book by Donahoo and Calvert.
The original code can be found at: http://cs.baylor.edu/~donahoo/practical/CSockets/code/HandleTCPClient.c
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>     /* for strcmp() */
#include "Database.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#define RCVBUFSIZE 32   /* Size of receive buffer */


// GET THE ONE LINE COMMAND FROM CLIENT
// DECODE IT AND DO WHAT HAS TO BE DONE
Database HandleCommand(Database Data, char* inputString, int sock, int servSock)
{	
	  // GET DATABASE
	  Database DB = Data;

	  // LOCAL VARIABLES
	  string user;
	  int * found;
	  // Holds the commandArgs
	  char* commandArgs[10];
	  int k = 0;
	  // To check if EOF is issued
	  char* comp = "EOF\0";
	  int command;
	  // Return to Client string
	  string retToC = "";
	  // String used to conver int to string
	  stringstream tempStr;

	  // INITIALIZE EMPTY ARRAY
	  while (k<9)
	  {
		commandArgs[k] = NULL;
		k++;
	  }
	  k = 0;

	  // OBJECT RECORD VARIABLES
	  int id;
	  int result;
	  string first;
	  string last;
	  char gender;
	  string location;

	  // BREAK DOWN THE LINE TO SINGLE WORD STRINGS
	  commandArgs[k] = strtok(inputString," ");
	  while (inputString != NULL)
	  {
		k++;
		inputString = strtok(NULL, " ");
	    commandArgs[k] = inputString;
	  }

	  // CHECK WHICH COMMAND IS ISSUED
	  string temp(commandArgs[0]);
	  if(temp.find("login")==0) 		command = 1;
	  else if(temp.find("quit")==0) 	command = 2;
	  else if(temp.find("add")==0) 		command = 3;
	  else if(temp.find("update")==0) 	command = 4;
	  else if(temp.find("remove")==0) 	command = 5;
	  else if(temp.find("find")==0) 	command = 6;
	  else if(temp.find("list")==0) 	command = 7;
	  else if(temp.find("locate")==0) 	command = 8;

	  // CALL THE COMMAND
	  switch (command) {
		case 1:
				// Command given is: login.
				user = string(commandArgs[1]);
				// Increment counters
				DB.totalPackets ++;
				DB.packets ++;
				DB.scripts ++;
				DB.clients ++;
				// Create return message
				retToC = "Hello ";
				retToC += user;
				retToC += "!";
				DB.user = user;
				// Give proper authorization levels
				if(user.find("FEMA")==0) DB.allowed = true;
						else			 DB.allowed = false;
				break;
		case 2:
				// "Command given is: quit.
				user = "";
				// Increment counters
				DB.commands ++;
				DB.packets ++;
				DB.scripts ++;
				DB.totalPackets ++;
				// Create return message
				retToC = "Bye ";
				retToC += DB.user;
				retToC += ". You issued ";
				tempStr << (DB.commands);
				retToC += tempStr.str();
				retToC += " commands! ";
				if(commandArgs[1]!=NULL) {
					if(strcmp(commandArgs[1],comp)==0) {
						stringstream tempStr3;
						retToC += " Total scripts processed: ";
						tempStr3 << (DB.scripts);
						retToC += tempStr3.str();
						cout << "ME Server handled: " << DB.clients << " client(s)! \n";
					}
						}
				cout << "Sent " << DB.packets << " TCP packets to " << DB.user << "\n";
				// Reset Database counters to default
				DB.user = "";
				DB.commands = 0;
			 	DB.packets = 0;
				DB.allowed = false;
				break;
		case 3:
				// Command given is: add.
				if(DB.allowed){
					// Initialize variable holders
					// Increment counters
					DB.scripts ++;
					DB.commands ++;
					id = atoi(commandArgs[1]);
					first = std::string(commandArgs[2]);
					first = first.substr(0,20);
					last = std::string(commandArgs[3]);
					last = last.substr(0,25);
					gender = (char) commandArgs[4][0];
					location = std::string(commandArgs[5]);
					location = location.substr(0,30);
					result = DB.add(id, first, last, gender, location);
					DB.sort();
					if(result>0)
						// If succesful return full record information
						{
						DB.totalPackets ++;
						DB.packets ++;
						retToC += commandArgs[1];
						retToC += " ";
						retToC += first;
						retToC += " ";
						retToC += last;
						retToC += " ";
						retToC += gender;
						retToC += " ";
						retToC += location;
						retToC += "\0";
						}
					}
				break;
		case 4:
				// Command given is: update.
				if(DB.allowed){
					// Initialize variable holders
					// Increment counters
					DB.commands ++;
					DB.scripts ++;
					DB.totalPackets ++;
					DB.packets ++;
					id = atoi(commandArgs[1]);
					first = std::string(commandArgs[2]);
					first = first.substr(0,20);
					last = std::string(commandArgs[3]);
					last = last.substr(0,25);
					gender = (char) commandArgs[4][0];
					location = std::string(commandArgs[5]);
					location = location.substr(0,30);
					result = DB.update(id, first, last, gender, location);
					DB.sort();
					if(result == 0)
						// If not found, return error message
						retToC += "Error: ID number not found! ";
						}
				break;
		case 5:
				// Command given is: remove.
				if(DB.allowed){
					// Initialize variable holders
					// Increment counters
					DB.commands ++;
					DB.scripts ++;
					DB.totalPackets ++;
					DB.packets ++;
					id = atoi(commandArgs[1]);
					result = DB.remove(id);
					DB.sort();
					// Return appropriate message
					if(result == 0)
						retToC += "Error: ID number not found. Nothing removed! ";
					else retToC += "Success: Entry removed. ";
					}
				break;
		case 6:
				// Command given is: find.
				// Initialize variable holders
				// Increment counters
				DB.commands ++;
				first = std::string(commandArgs[1]);
				first = first.substr(0,20);
				last = std::string(commandArgs[2]);
				last = last.substr(0,25);
				found = DB.find(first, last);
				k = 0;
				DB.scripts ++;
				while((k < 99) && found[k]>=0)
				{	
					// Create appropriate return message
					DB.totalPackets ++;
					DB.packets ++;
					stringstream tempStr1;
					tempStr1 << (DB.recordDB[found[k]].id_number);
					retToC += tempStr1.str();
					retToC += " ";
					retToC += DB.recordDB[found[k]].first_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].last_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].gender;
					retToC += " ";
					retToC += DB.recordDB[found[k]].location;
					retToC += " ";
					retToC += " \n";
					k++;
				}
				
				k = 0;
				break;
		case 7:
				// Command given is: list.
				k = 0;
				// Initialize variable holders
				// Increment counters
				DB.scripts ++;
				DB.commands ++;
				last = 'z';
				first = 'a';
				if(commandArgs[1]!=NULL) {
					first = std::string(commandArgs[1]);
					first = first.substr(0,20);
					k++;}
				if(commandArgs[2]!=NULL) {
					last = std::string(commandArgs[2]);
					last = last.substr(0,25);
					k++;}
				// If error, return appropriate message
				if(last.compare(first) < 0) { 
					  retToC += "Error: Finish character comes before start character! ";
					  DB.totalPackets ++;
					  DB.packets ++;}
				else{
				if (k==0) found = DB.list();
				if (k==1) found = DB.list(first);
				if (k==2) found = DB.list(first, last);
				k = 0;
				while((k < 99) && found[k]>=0)
				{	
					// Return all appropriate fields and create
					// the return message for the client
					DB.totalPackets ++;
					DB.packets ++;
					stringstream tempStr1;
					tempStr1 << (DB.recordDB[found[k]].id_number);
					retToC += tempStr1.str();
					retToC += " ";
					retToC += DB.recordDB[found[k]].first_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].last_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].gender;
					retToC += " ";
					retToC += DB.recordDB[found[k]].location;
					retToC += " ";
					retToC += " \n";
					k ++;
				}}
				k = 0;
				break;
		case 8:
				// Command given is: locate.
				// Initialize variable holders
				// Increment counters	
				DB.scripts ++;
				DB.commands ++;
				location = std::string(commandArgs[1]);
				location = location.substr(0,30);
				found = DB.locate(location);
				k = 0;
				while((k < 99) && found[k]>=0)
				{	
					// Return all appropriate fields and create
					// the return message for the client
					DB.totalPackets ++;
					DB.packets ++;
					stringstream tempStr1;
					tempStr1 << (DB.recordDB[found[k]].id_number);
					retToC += tempStr1.str();
					retToC += " ";
					retToC += DB.recordDB[found[k]].first_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].last_name;
					retToC += " ";
					retToC += DB.recordDB[found[k]].gender;
					retToC += " ";
					retToC += DB.recordDB[found[k]].location;
					retToC += " ";
					retToC += "\n";
					k ++;
				}

				k = 0;
				break;
		default:
				cout << "Invalid Command.\n";
	  }

	  // return response message to Client
	  char * returnToClient = (char*) (retToC.substr(0,512)).c_str();

	   send(sock, returnToClient, 512, 0);

	  return DB;
}
