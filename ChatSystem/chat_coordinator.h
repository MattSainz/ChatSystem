#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "uthash.h"

#define BUF_SIZE 80
#define START "Start"
#define FIND "Find"
#define TERM "Terminate"

struct hash
{
  char* s_name;

};

/*
 * creates socket
 */
int create_socket();

/*
 * creates a new UDB connection and binds
 * it to the port we created
 */
sockaddr_in new_connection(int socket);

/*
 * Create a chat server if the secession doesn't already exist 
 * if one already does then -1 is returned 
 * otherwise a TCP port socket is created and a new server is started 
 * a message is returned to the client with the address of the TCP server
 * and the port number
 */
void my_start( char* s_name, char* to_client );

/* 
 * Looks for a server with the name passed in 
 * if found then returnes the server's address (port)
 * otherwise -1 is returned
 */
void my_find( char* s_name, char* to_client );

/*
 * Terminates the chat session
 */
void my_terminate( char* s_name, char* to_client );

/*
 * Waits for clients to call and then 
 * calls calls the correct method
 */
void run_chat_coordinator();

