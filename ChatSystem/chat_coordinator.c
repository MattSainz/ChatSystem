/* 
 * File:   chat_coordinator.c
 * Author: Matthias
 *
 * Created on September 15, 2014, 3:13 PM
 */

#include "chat_coordinator.h"

Vector* running_sessions;
    //stores all of the currently running sessions 

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

void new_connection(int connection_type, Connection *to_ret)
{ 
  struct sockaddr_in my_address;
    //holds connection information
  
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
    to_ret->c.sin_family = AF_INET;
      //sets connection type or connection family 
    to_ret->c.sin_addr.s_addr = htonl(INADDR_ANY);
      //local host address htonol creates network format
    to_ret->c.sin_port = htons(0);
      //a port of zero asks the os to pick a port for us
      //htons formats network format

    if(bind(my_socket, (struct sockaddr*) &my_address, sizeof(my_address)) < 0)
    {
      to_ret->c.sin_port = -1;
      perror("Bind failed");
    }//end bind socket 
  }
  else
  {
    to_ret->c.sin_port = -1;
  }

}//end bind socket

void start_new_session(int port)
{
    int pid;
    if( (pid = fork()) == 0)
    {
      printf("Hello From Child");
      //TODO create session server and pass it the tcp port
      exit(0);
    }
    else if( pid == -1 )
    {
      perror("Error Forking");
    }
}

//TODO fix this horrible horrible memory leak
//TODO add tcp error checking
void my_start(char* s_name, char* to_client)
{
  if( my_find(s_name) == NULL )
  {
    Connection tcp_connection;
    Node* to_array;
    char* tmp;
    new_connection(0, &tcp_connection);
      //TODO create session server
    tmp = (char*)malloc(sizeof(s_name));
      //allocate space for the name
    tmp = s_name;
      //store the name
    to_array = (Node*)malloc(sizeof(tmp));
      //make memory for this new information
    to_array->session = tmp;
      //set the memory 
    to_array->port = tcp_connection.socket;
    
    vector_append(running_sessions, to_array);
    
    sprintf(to_client, "%d", tcp_connection.c.sin_port);
      //set the message to be sent back to the client
  }
  else
  {
    to_client = "-1";
      //error message to be sent back to the client
  }//end unique session check
  
}//end my start

 Node* my_find(char* s_name)
{
  int i = 0;
    //index into vector
  Node* to_ret = NULL;
    //the session does not already exist
  Node* session;
  
  while( i < running_sessions->size)
  {
    session = vector_get(running_sessions, i);
    
    if( strcmp(s_name, session->session) == 0 )
    {
      to_ret = session;
        //a session with the same name was found  
        //note returning a pointer should work 
        //because it was stored on the heap earlier
      break;
        //you only need to find one session 
    }
    i++;
  }
  
  return to_ret;
}//end my find

void my_terminate(char* s_name, char* to_client)
{
}//end my terminate

void run_chat_coordinator()
{
  Connection myself;
  
  struct sockaddr_in client_address;
    //stores information about the client
  socklen_t addrlen = sizeof(client_address);
    //stores the length of the client address
  char buf[NAME_SIZE];
    //will store the message from the client
  char* to_client;
    //will store the message to be sent to the client
  int msg_len = 0;
    //stores the length of the message from the client
  new_connection(1, &myself);
    //attempt to bind socket
  int socket = myself.socket;
    //get socket
  printf("UDP Port Number: %d \n", myself.c.sin_port);
    //output the port number of the new connection
  
  vector_init(running_sessions);
    //inits vector that stores all sessions

  if( DEBUG != 1)
  {
    while(1)
    {
      msg_len = recvfrom(socket, buf, BUF_SIZE, 0, 
                        (struct sockaddr *)&client_address, &addrlen);
        //wait for client to send a message

      if(strcmp(buf, START) == 0)
      {
        my_start(buf, to_client);
      }
      else if(strcmp(buf, FIND) == 0)
      {
        my_find(buf);
      }
      else if(strcmp(buf, TERM) == 0)
      {
        my_terminate(buf, to_client);
      }
      else
      {
        perror("Unknown msg received");
      }//determine action to take based on client request

    }//listen for commands
  }
  
}//end run_chat_coordinator

/*
 * Standard main method
 */
int main(int argc, char** argv)
{  
  run_chat_coordinator();
}//end main



