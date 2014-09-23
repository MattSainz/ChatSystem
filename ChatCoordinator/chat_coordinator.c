/* 
 * File:   chat_coordinator.c
 * Author: Matthias
 *
 * Created on September 15, 2014, 3:13 PM
 */

#include "chat_coordinator.h"

Dict sessions;

int create_socket(int type, int protocol)
{
  int to_ret = 0;
  
  if ((to_ret = socket(AF_INET, type, protocol)) < 0) 
  {
    perror("cannot create socket");
    to_ret = -1;
  }
  
  return to_ret;
}//end create socket

Connection* new_connection(int connection_type)
{ 
  struct sockaddr_in my_address;
    //holds connection information
  
  Connection *to_ret;
  
  to_ret = (Connection*)malloc(sizeof(Connection));
  
  int my_socket;
  if( connection_type == 1)
  {
     my_socket = create_socket(SOCK_DGRAM, IPPROTO_UDP);
      //attempt to create a socket
  }
  else
  {
    my_socket = create_socket(SOCK_STREAM,IPPROTO_TCP);
  }
  
  to_ret->socket = my_socket;

  if( my_socket != -1 )
  {
    bzero(&my_address, sizeof(my_address));
      //allocates memory
    
    my_address.sin_family = AF_INET;
      //sets connection type or connection family 
    my_address.sin_addr.s_addr = htonl(INADDR_ANY);
      //local host address htonol creates network format
    my_address.sin_port = htons(0);
      //a port of zero asks the os to pick a port for us
      //htons formats network format

    if(bind(my_socket, (struct sockaddr*) &my_address, sizeof(my_address)) < 0)
    {
      my_address.sin_port = -1;
      perror("Bind failed");
    }//end bind socket 
  }
  else
  {
    my_address.sin_port = -1;
  }
  
  to_ret->port = my_address.sin_port;
  
  return to_ret;
}//end bind socket

void start_new_session(int port , int socket)
{
    int pid;
    char* args[4];
    args[0] = SERVER_NAME;
    args[3] = '\0';
    int len;

    //store port in arg array
    if( port != 0 )
    {
      char port_s[(len = (int)((ceil(log10(socket))+1)*sizeof(char)))];
      snprintf(port_s, len, "%d", port);
      args[1] = port_s;
    }
    else
    {
      args[1] = "0";
    }
    
    //store socket in arg array 
    if( socket != 0 )
    {
      char socket_s[(len = (int)((ceil(log10(socket))+1)*sizeof(char)))];
      snprintf(socket_s, len, "%d", socket);
      args[2] = socket_s;   
    }
    else
    {
      args[2] = "0";
    }
    
    printf("Port: %s Socket: %s \n", args[1], args[2]);
    if( (pid = fork()) == 0)
    {
      printf("Port: %s Socket: %s \n", args[1], args[2]);
      chdir(SERVER_PATH);
      
      if ( execv(SERVER_NAME, args) < 0 )
      {
        printf("Error making child \n");
      }
      
      exit(-1);
    }
    else if( pid == -1 )
    {
      perror("Error Forking\n");
    }
    printf("In parent \n");
}

//TODO fix this horrible horrible memory leak
//TODO add tcp error checking
int my_start(char* s_name)
{
  int to_ret = -1;
  
  if( my_find(s_name) == -1 )
  {
    Connection* tcp_connection = new_connection(0);    
      //TODO create session server
    DictInsert(sessions, s_name, tcp_connection->port);
    
    start_new_session(tcp_connection->port, tcp_connection->socket);
    
    if( tcp_connection != NULL ) free(tcp_connection);
    
    to_ret = 1;
  }//end check for existing session
  
  return to_ret;
}//end my start

 int my_find(char* s_name)
{
  int to_ret = -1;
    //the session does not already exist
  if( DictSearch(sessions,s_name) != 0 )
  {
    to_ret = 1;
  }
  return to_ret;
}//end my find

int my_terminate(char* s_name)
{
  int to_ret = -1;  
  DictDelete(sessions,s_name);
  if( DictSearch(sessions, s_name) != -1)
  {
    to_ret = 1;
  }
  return to_ret;
}//end my terminate

void run_chat_coordinator()
{
  Connection* myself;
  
  struct sockaddr_in client_address;
    //stores information about the client
  socklen_t addrlen = sizeof(client_address);
    //stores the length of the client address
  char buf[NAME_SIZE];
    //will store the message from the client
  char* to_ret;
    //will store the message to be sent to the client
  int msg_len = 0;
    //stores the length of the message from the client
  myself = new_connection(1);
    //attempt to bind socket
  int socket = myself->socket;
    //get socket
  printf("UDP Port Number: %d Socket: %d\n", myself->port, myself->socket);
    //output the port number of the new connection
  
  if( myself != NULL ) free(myself);

  sessions = DictCreate();
  
  if( DEBUG != 1)
  {
    while(1)
    {
      msg_len = recvfrom(socket, buf, BUF_SIZE, 0, 
                        (struct sockaddr *)&client_address, &addrlen);
        //wait for client to send a message

      if(strcmp(buf, START) == 0)
      {
     
        if( my_start(buf) != -1 )
        {
            sprintf(to_ret,"%d",DictSearch(sessions, buf));
        }
        else
        {
            to_ret = -1;
        }
      }
      else if(strcmp(buf, FIND) == 0)
      {
        sprintf(to_ret,"%d",my_find(buf));    
      }
      else if(strcmp(buf, TERM) == 0)
      {
        my_terminate(buf);
      }
      else
      {
        perror("Unknown msg received");
      }//determine action to take based on client request

    }//listen for commands
  }
  else
  {
    char * foo = "test";
    int test_start = my_start(foo);
    if( test_start != -1 )
    {
        printf("Start All good \n");
    }
    else
    {
        printf("Start Not so Good \n");
    }
    
    int test_find = my_find(foo);
    if( test_find != 0 )
    {
        printf("Dict and find working \n");
    }
    else
    {
        printf("Dict and find not so good \n");
    }
    
    int test_term = my_terminate(foo);
    if( test_term != -1 )
    {
        printf("Term working \n");
    }
    else
    {
        printf("Term not so good \n");
    }
      
  }
  
  
}//end run_chat_coordinator

/*
 * Standard main method
 */
int main(int argc, char** argv)
{  
  run_chat_coordinator();
}//end main



