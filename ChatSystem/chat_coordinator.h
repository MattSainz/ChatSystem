#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "uthash.h"
#include "list.h"

#define BUF_SIZE 80
#define NAME_SIZE 8
#define START "Start"
#define FIND "Find"
#define TERM "Terminate"
#define DEBUG 1
#define running_sessions

typedef struct 
{
  struct sockaddr_in c;
  int socket;
}Connection;

typedef struct
{
  int port;
  char* s_name;
}Node;

/*
 * creates socket
 */
int create_socket(int type, int protocol);

/*
 * creates a new UDB connection and binds
 * it to the port we created
 */
void new_connection(int connection_type, Connection* to_ret);

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
Node* my_find( char* s_name);

/*
 * Terminates the chat session
 */
void my_terminate( char* s_name, char* to_client );

/*
 * Create a new SessionServer
 */
void start_new_session(int port);

/*
 * Waits for clients to call and then 
 * calls calls the correct method
 */
void run_chat_coordinator();

