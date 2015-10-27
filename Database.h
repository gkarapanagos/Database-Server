//file song.h
#include <stdlib.h>
#include <iostream>
using namespace std;

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <string.h>

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);


class Record{
	public:
		Record(int id_number, string first_name, string last_name, char gender, string location);
		Record();
		void setValues(int id_number, string first_name, string last_name, char gender, string location);
		int id_number;
		string first_name;
		string last_name;
		char gender;
		string location;
};

class Database
{
	public:
	  // Default constructor
	  Database();
	  // Adds a new record
	  int add(int id_number, string first_name, string last_name, char gender, string location);
	  // Updates an existing record
	  int update(int id_number, string first_name, string last_name, char gender, string location);
	  // Removes an existing record
	  int remove(int id_number);
	  // Alphabetically sorts the database
	  void sort();
	  // Helper swap function for sort
	  void swap(int index);
	  // Returns entries with given first and last name
	  int* find(string first_name, string last_name);
	  // Lists all entries whose last name start with any character ranging from the first to the second argument
	  int* list(string start, string finish);
	  // Lists all entries whose last name start with any character ranging from the first to the end of the database
	  int* list(string start);
	  // Lists all entries
	  int* list();
	  // List all entries whose location matches the argument
	  int* locate(string location);
	  // Array of records
	  Record recordDB[100];
	  // Database's fields
	  // Including counters, and authorization indicator
	  bool allowed;
	  string user;
	  int scripts;
	  int packets;
	  int commands;
	  int clients;
	  int totalPackets;
};

// Handles commands issued by the server, received from the client
Database HandleCommand(Database DB, char* inputString, int sock, int servSock);
