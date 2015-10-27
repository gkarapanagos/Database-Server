/*
Author: Georgios Karapanagos - gkarapanagos
Date: 09/01/2015

This is the function that contains the error messages.

Code taken from the "TCP/IP Sockets in C" book by Donahoo and Calvert.
The original code can be found at: http://cs.ecs.baylor.edu/~donahoo/practical/CSockets2/code/code/DieWithMessage.c
*/

#include <stdio.h>
#include <stdlib.h>

void DieWithUserMessage(const char *msg, const char *detail) {
	// print out the error message and exit
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DieWithSystemMessage(const char *msg) {
	// print out the error message and exit
	perror(msg);
	exit(1);
}
