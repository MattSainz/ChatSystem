/* 
 * File:   chat_client.h
 * Author: Matthias
 *
 * Created on September 24, 2014, 1:06 AM
 */

#ifndef CHAT_CLIENT_H
#define	CHAT_CLIENT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/errno.h>
#include <arpa/inet.h>
    
#define MSG_SIZE 80
#define NAME_SIZE 8
#define GET_NEXT "1"
#define GET_ALL  "2"
#define START    "3"
#define SUBMIT   "4"
#define JOIN     "5"
#define FIND     "6"
#define LEAVE    "7"

  char *current_session;
  
  struct sockaddr_in addr, remote_addr; /* an Internet endpoint address         */
  
  int id; 

  char* server_port;
  char* server_host;

  char* coord_port;
  char* coord_host;

  int tcp_socket;
  /*
   * Sends start message to the chat coordinator 
   * if there is an error the appropriate message is printed 
   * otherwise the new session is joined and the result of doing so 
   * is printed to the screen. Listens for join command then calls the
   * session_join method
   */
  void session_start(char* s_name);
  
  /*
   * Sends find to the coordinator and then uses those credentials 
   * to join a session
   */
  void session_join(char* s_name );
  
  /*
   * Sends message to chat server
   */
  void submit(char* msg);
  
  /*
   * either prints new message or displays no new message 
   */
  void get_next();
  
  /*
   * prints all messages one at a time
   */
  void get_all();
  
  /*
   * Leaves the current chat session
   */
  void leave();
  
  /*
   * Ends the program
   */
  void session_exit();
  
  /*
   * Responsible for actual communication with the 
   * server and client
   */
  char* send_command(char* command, char* msg);
  
  /*
   * Handles communication with user 
   * and calls the correct commands
   */
  void runner();
  
  void connect_sock(const char* ip, const char* port, int con_type);
  
  int errexit(const char* format, ...);

  char* send_command(char* command, char* msg);
    
#ifdef	__cplusplus
}
#endif

#endif	/* CHAT_CLIENT_H */

