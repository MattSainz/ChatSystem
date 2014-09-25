#ifndef CHAT_COORDINATOR_H
#define CHAT_COORDINATOR_H

#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>
#include <netinet/in.h>
#include "dict.h"

#define LEN(x) (sizeof(x)/ sizeof(x[0]))
    
#define MSG_SIZE 80
#define NAME_SIZE 8
#define START    "3"
#define JOIN     "5"
#define FIND     "6"
#define LEAVE    "7"
#define DEBUG 0
#define running_sessions
#define SERVER_PATH "/home/user/Code/Git/ChatSystem/Build"
#define SERVER_NAME "./sessionserver"

typedef struct 
{
  int port;
  int socket;
}Connection;

/*
 * creates socket
 */
int create_socket(int type, int protocol);

/*
 * creates a new UDB connection and binds
 * it to the port we created
 */
Connection* new_connection(int connection_type);

/*
 * Create a chat server if the secession doesn't already exist 
 * if one already does then -1 is returned 
 * otherwise a TCP port socket is created and a new server is started 
 * a message is returned to the client with the address of the TCP server
 * and the port number
 */
int my_start( char* s_name);

/* 
 * Looks for a server with the name passed in 
 * if found then returnes the server's address (port)
 * otherwise -1 is returned
 */
int my_find( char* s_name);

/*
 * Terminates the chat session
 */
int my_terminate( char* s_name);

/*
 * Create a new SessionServer
 */
void start_new_session(int port, int socket);

/*
 * Waits for clients to call and then 
 * calls calls the correct method
 */
void run_chat_coordinator();


#ifdef _cplusplus
#endif

#endif
