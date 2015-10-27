// classes example
#include <iostream>
#include <string>
using namespace std;
#include "Database.h"

// Record constructor with variables to initialize fields
Record::Record(int id, string first, string last, char g, string loc){
	id_number = id;
	first_name = first;
	last_name = last;
	gender = g;
	location = loc;
}

// Default Record constructor
Record::Record()
	{		id_number = 0;
			first_name = "";
			last_name = "zzzzzzzzzzzzzzzzzzzzzzzzz";
			// last_name chosen for ordering purposes
			gender = '0';
			location = "";}

// Setter method for Record
void Record::setValues(int id, string first, string last, char g, string loc){
	id_number = id;
	first_name = first;
	last_name = last;
	gender = g;
	location = loc;
}

// Default Database constructor
Database::Database(){
	  allowed = false;
	  user = "";
	  scripts = 0;
	  packets = 0;
	  totalPackets = 0;
	  commands = 0;
	  clients = 0;
}

// Adding a record to the database
int Database::add(int id_number, string first_name, string last_name, char gender, string location){
	int k = 0;
	bool found = false;
	while(k<99){
		if(id_number == recordDB[k].id_number) found = true;
		k++;}
	k = 0;

	// If record with identical id is not found
	if(!found){
			// LOOK FOR THE FIRST EMPTY RECORD
			while(recordDB[k].id_number != 0) {k++;}
			// CREATE RECORD
			Record* temp = new Record(id_number, first_name, last_name, gender, location);
			// ADD IT IF LIST NOT FULL
			if(k<100) recordDB[k] = *temp;
			// Return that succesfully added
			return 1;
	}

	// If ID exists or database is full
	return 0;
}

// Updating a record from the database
int Database::update(int id_number, string first_name, string last_name, char gender, string location){
	bool found = false;
	int k = 0;
	// FIND THE RECORD
	while(recordDB[k].id_number != id_number && k<99) {k++;}
	// IF FOUND
	if(recordDB[k].id_number == id_number) found = true;
	// UPDATE IT
	if(found==true){
		recordDB[k].id_number = id_number;
		recordDB[k].first_name = first_name;
		recordDB[k].last_name = last_name;
		recordDB[k].gender = gender;
		recordDB[k].location = location;
		return 1;
	}
	// Else return that it wasn't found
	return 0;
}

// Removes a record from the database
int Database::remove(int id_number){
	bool found = false;
	int k = 0;
	// FIND THE RECORD
	while(recordDB[k].id_number != id_number && k<99) {k++;}
	// IF FOUND
	if(recordDB[k].id_number == id_number) found = true;
	// REMOVE IT
	if(found==true){
		recordDB[k].id_number = 0;
		recordDB[k].first_name = "";
		recordDB[k].last_name = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
		recordDB[k].gender = '0';
		recordDB[k].location = "";
		return 1;
	}
	// Else return that deletion was unsuccessful
	return 0;
}

// Finds all entries with given first and last name
int* Database::find(string first_name, string last_name){
	int k = 0;
	int i = 0;
	int *result;
	result = new int[99];
	string UNK = "UNK";

	// INITIALIZE ALL RESULTS TO NEGATIVE
	while(k<99){
		result[k] = -1;
		k++;}

	k = 0;

	// FIND ALL MATCHING RESULTS
	while(k<99)
	{	// IF IT's A VALID ENTRY
		if( (recordDB[k].id_number != 0 ) &&
		// AND IF FIELDS MATCH
		(first_name.find(recordDB[k].first_name) == 0) && (last_name.find(recordDB[k].last_name) == 0) ){
				// SET RECORD K IS ONE OF THE RESULTS
				result[i] = k;
				// INCREMENT FOUND COUNTER
				i++;}

		// INCREMENT LOOP COUNTER
		k++;}

	// Result holds the indexes of all Records that should be returned
	return result;
}

// Returns all entries whose last name start with the first argument up to second argument
int* Database::list(string start, string finish){
	int i = 0;
	int k = 0;
	int *result;
	result = new int[99];
	string tempStart;
	string tempFinish;
	string UNK = "UNK";
	// INITIALIZE ALL RESULTS TO NEGATIVE
	while(k<99){
		result[k] = -1;
		k++;}

    for(k=0; k<99; k++){
    	// UNK are not returned
    	if(recordDB[k].id_number>0 
    		&& UNK.find(recordDB[k].first_name) != 0 
    		&& UNK.find(recordDB[k].last_name) != 0){
	    	tempStart = recordDB[k].first_name[0];
	    	tempFinish = recordDB[k].last_name[0];
	    	// If between start and finish character return them
	    	if(recordDB[k].last_name.compare(start) > 0 &&
	    	   finish.compare(tempFinish) > 0){
	    		result[i] = k;
	    		i++;
    	}
    }}

    // Result holds the indexes of all Records that should be returned
	return result;
}

// Returns all entries whose last name start with the 
int* Database::list(string start){
	int i = 0;
	int k = 0;
	int *result;
	result = new int[99];
	string UNK = "UNK";
	// INITIALIZE ALL RESULTS TO NEGATIVE
	while(k<99){
		result[k] = -1;
		k++;}

    for(k=0; k<99; k++){
    	// UNK are not returned
    	if(recordDB[k].id_number>0 
    		&& UNK.find(recordDB[k].first_name) != 0 
    		&& UNK.find(recordDB[k].last_name) != 0){
    		// If alphabetically higher than the start character
	    	if(recordDB[k].last_name.compare(start) > 0) {
	    		result[i] = k;
	    		i++;
    	}
    }}

    // Result holds the indexes of all Records that should be returned
	return result;
}

// Return all database entries, except for unidentified bodies.
int* Database::list(){
	int i = 0;
	int k = 0;
	int *result;
	result = new int[99];
	string UNK = "UNK";
		// INITIALIZE ALL RESULTS TO NEGATIVE
	while(k<99){
		result[k] = -1;
		k++;}

	k = 0;

	while(k<99){
		// UNK are not returned
		if(recordDB[k].id_number > 0 
		&& UNK.find(recordDB[k].first_name) != 0 
    	&& UNK.find(recordDB[k].last_name) != 0){
			result[i] = k;
			i++;
	}	
		k++;
	}

	// Result holds the indexes of all Records that should be returned
	return result;
}

// Returns all entries whose location is same as the passed argument
int* Database::locate(string location){
	int i = 0;
	int k = 0;
	int *result;
	result = new int[99];
	string UNK = "UNK";
	// INITIALIZE ALL RESULTS TO NEGATIVE
	while(k<99){
		result[k] = -1;
		k++;}
	string tmp1;
	std::transform(location.begin(), location.end(), location.begin(), ::tolower);

    for(k=0; k<99; k++){
    	// If not UNK and if Location case-insesitive is the same as argument
    	// Return this entry
    	if(UNK.find(recordDB[k].first_name) != 0 
    	&& UNK.find(recordDB[k].last_name) != 0){
    		tmp1 = recordDB[k].location;
			std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::tolower);
	    	if(location.find(tmp1) == 0){
	    		result[i] = k;
	    		i++;
    	}
    }}

    // Result holds the indexes of all Records that should be returned
	return result;
}

// Sorts the database alphabetically, case-insensitive and puts UNK to the end
void Database::sort(){
	int i, k;
	string tmp1, tmp2;
	string UNK = "unk";
	// Iterate through the whole database
	for(k=0; k<99; k++){
		for(i=0; i<98; i++){
			// Get the first last name in lower case
			tmp1 = recordDB[i].last_name;
			std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::tolower);
			// Get the second last name in lower case
			tmp2 = recordDB[i+1].last_name;
			std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::tolower);
			// Compare them
			if((tmp1.compare(tmp2) > 0 && UNK.find(tmp2) != 0) ||
			   (tmp1 == "unk" && recordDB[i+1].id_number> 0) )
		 	// And if necessary swap them 
				swap(i);
		}
	}
}

// Helper Swap function for the Sorting algorithm
void Database::swap(int index){
	// Temporary holders
	Record* temp;
	int idTemp;
	string firstTemp;
	string lastTemp;
	char gTemp;
	string locTemp;

	// Hold the first Record
	temp = &recordDB[index]; 

	// Hold the second Record's values
	idTemp = recordDB[index+1].id_number;
	firstTemp = recordDB[index+1].first_name;
	lastTemp = recordDB[index+1].last_name;
	gTemp = recordDB[index+1].gender;
	locTemp = recordDB[index+1].location;

	// Do the swap
	recordDB[index+1].setValues((*temp).id_number,
								(*temp).first_name,
								(*temp).last_name,
								(*temp).gender,
								(*temp).location);

	recordDB[index].setValues(idTemp, firstTemp, lastTemp, gTemp, locTemp);
}



