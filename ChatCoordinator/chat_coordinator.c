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

    if(bind(my_socket, (struct sockaddr*) &my_address, sizeof(my_address)) == -1)
    {
      printf("Error binding socekt \n");
      to_ret->port = -1;
    }//end bind socket 
    socklen_t len = sizeof(my_socket);
    getsockname(my_socket, (struct sockaddr *)&my_address, &len);
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
  args[3] = NULL;

  char port_s[200];
  sprintf(port_s,"%d",port);
  printf("Port: %s \n", port_s);

  char socket_s[200];
  sprintf(socket_s,"%d", socket);
  printf("Socket: %s\n", socket_s);

  args[1] = port_s;
  args[2] = socket_s;

  if( (pid = fork()) == 0)
  {
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
  printf("Pid of server: %d \n",pid);
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
  char buf[MSG_SIZE];
    //will store the message from the client
  char to_ret[MSG_SIZE];
  char *command; 
  char* arg; 
  char* tmp;
  
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
      printf("Waiting For connection \n");
      while ( (msg_len = recvfrom(socket, buf, MSG_SIZE, 0, 
                        (struct sockaddr *)&client_address, &addrlen)) != -1)
      {
        printf("Got message \n");
        //wait for client to send a message
        tmp = buf;

        command = strsep(buf, ",");
        printf("Command: %s \n", command);

        arg     = strsep(buf, ",");
        printf("Arg: %s \n", arg);

        if(strcmp(command, START) == 0)
        {

          if( my_start(arg) != -1 )
          {
            sprintf(to_ret,"%d",DictSearch(sessions, arg));
          }
          else
          {
            sprintf(to_ret,"%d",-1);
          }
          printf("Worked %s: \n", to_ret);
          sendto(socket, to_ret, sizeof(to_ret), 0, (struct sockaddr *) &client_address,
                  sizeof(struct sockaddr_in));
        }
        else if(strcmp(command, FIND) == 0)
        {
          sprintf(to_ret,"%d",my_find(arg));    
        }
        else if(strcmp(command, LEAVE) == 0)
        {
          my_terminate(arg);
        }
        else
        {
          perror("Unknown msg received");
        }//determine action to take based on client request
      }
    }//listen for commands
    
    if( buf != NULL ) free(buf);
  }
  else
  {
    my_start("test");
  }
  
}//end run_chat_coordinator

/*
 * Standard main method
 */
int main(int argc, char** argv)
{  
  run_chat_coordinator();
}//end main



