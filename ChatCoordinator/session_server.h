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

#define BUFSIZE 80
  //max message length
#define MAXPENDING 100
  //max pending connections
#define BUF 80 

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
void submit(char* id, char* msg, int socket);

/*
 * Gets the next message that has not been read by
 * this client from the chat session and sends to client
 */
void get_next(char* client, int socket);

/* 
 * Gets all of the messages not read by the client
 * and first sends the number of new chat messages 
 * then sends the messages and their length
 */
void get_all(char* client, int socekt);

/*
 * Closes the TCP connection with the client
 */
void leave(char* client, int socket);

/*
 * Listens for a client then handles
 * the request with the proper method
 */
void my_wait();

/*
 * Takes the client and figures out what to do 
 * with it
 */
int process(int socket);

/*
 * Inits data structures for server
 */
void init();

/*
 * Assigns a unique id to a client to 
 * allow for identification of different clients on the 
 * same network
 */
void join(int socket);

/*
 * appends a message to a clients history
 */
void history_add(char* id);

/*
 * Checks if a message was sent from the client 
 */
int msg_in_history(char* id, int msg);