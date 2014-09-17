/* 
 * File:   chat_coordinator.c
 * Author: Matthias
 *
 * Created on September 15, 2014, 3:13 PM
 */

#include "chat_coordinator.h"

int create_socket()
{
  int to_ret = 0;
  if ((to_ret = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
  {
    perror("cannot create socket");
    to_ret = -1;
  }
  return to_ret;
}//end create socket

sockaddr_in new_connection(int socket)
{
  struct sockaddr_in my_address;
    //holds connection information
  int my_socket = create_socket();

  if( my_socket == -1 )
  {
    return -1;
  }

  bzero(&my_address, sizeof(my_address));
    //allocates memory

  my_address.sin_family = AF_INET;
    //sets connection type or connection family 
  my_address.sin_addr.s_addr = htonl(INADDR_ANY);
    //local host address htonol creates network format
  my_address.sin_port = htons(0);
    //a port of zero asks the os to pick a port for us
    //htons formats network format

  if(bind(my_socket(), (struct sockaddr*) &my_address, sizeof(my_address)) < 0)
  {
    perror("Bind failed");
    return -1;
  }//end bind socket
  return my_address;
}//end bind socket

void my_start(char* s_name, char* to_client)
{
}//end my start

void my_find(char* s_name, char* to_client)
{
}//end my find

void my_terminate(char* s_name, char* to_client)
{
}//end my terminate

void run_chat_coordinator()
{
  struct sockaddr_in client_address;
    //stores information about the client
  socklen_t addrlen = sizeof(client_address);
    //stores the length of the client address
  unsigned char buf[BUF_SIZE];
    //will store the message from the client
  int msg_len = 0;
    //stores the length of the message from the client

  int socket = create_socket();
    //attempt to create a socket
  sockaddr_in myself = new_connection(socket);
    //attempt to bind socket
  printf("UDP Port Number: %d", myself.sin_port);
    //output the port number of the new connection

  while(1)
  {
    msg_len = recvfrom(socket, buf, BUF_SIZE, 0, 
                      (struct sockaddr *)&client_address, &addrlen);

    if(strcmp(buf,START) == 0)
    {
    }
    else if(strcmp(buf, FIND) == 0)
    {
    }
    else if(strcmp(buf, TERM) == 0)
    {
    }
    else
    {
      perror("Unknow msg received");
    }

  }//listen for commands
}//end run_chat_coordinator

/*
 * Standard main method
 */
int main(int argc, char** argv)
{  
  return (EXIT_SUCCESS);
}//end main



