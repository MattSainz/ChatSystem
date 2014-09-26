/* 
 * File:   session_server.h
 * Author: Matthias 
 * 
 * Stores chat history and the last message read by a client
 * 
 * Has two dicts one storing all of the messages 
 * and another that maps clients to keys of their last unread message
 *
 * Created on September 22, 2014, 1:04 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "dict.h"
#include "dict_1.h"

#define BUFSIZE 80
  //max message length
#define MAXPENDING 100
  //max pending connections
#define BUF 1024

#define GET_NEXT "1"
#define GET_ALL  "2"
#define START    "3"
#define SUBMIT   "4"
#define JOIN     "5"
#define FIND     "6"
#define LEAVE    "7"

int tcp_socket;
  //socket made by session server 
int tcp_port;
  //port made by session server
int num_msg;
  //stores the number of messages currently on the server
int client_id;
  //client

Dict messages;
  //stores all messages send to server inorder 
Dict clients;
  //maps clients to last message recived 
Dict msg_history;
  //stores the messages send by each individual clinet 

/*
 * Submits a message to the chat session must be 
 * ended with at <CR> 
 */
void submit(char* id, char* msg);

/*
 * Gets the next message that has not been read by
 * this client from the chat session and sends to client
 */
void get_next(char* client);

/* 
 * Gets all of the messages not read by the client
 * and first sends the number of new chat messages 
 * then sends the messages and their length
 */
void get_all(char* client);

/*
 * Closes the TCP connection with the client
 */
void leave(char* client);

/*
 * sends a message to the specified client
 */
void my_send(char *msg, int len, int client);

/*
 * Listens for a client then handles
 * the request with the proper method
 */
void my_wait();

/*
 * Takes the client and figures out what to do 
 * with it
 */
void process(int socket);

/*
 * Inits data structures for server
 */
void init();

/*
 * Assigns a unique id to a client to 
 * allow for identification of different clients on the 
 * same network
 */
int join();

/*
 * appends a message to a clients history
 */
void history_add(char* id);