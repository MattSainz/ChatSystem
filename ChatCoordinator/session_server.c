/*
 * Session Server by Matthias Sainz
 */
#include "session_server.h"

void submit(char* msg, int mgs_len)
{
    
}//end submit

void get_next(int client)
{
    
}//end get_next

void get_all(int client)
{
    
}//end get_all

void leave(int client)
{
    
}//end leave

void my_send(char *msg, int len, int client)
{
    
}//end send

void my_wait(int socket)
{
  printf("Waiting for connections on %d \n",socket);
  struct sockaddr_in client;
  int client_socket;
  int client_len = sizeof(client);  
  
  while(1)
  {
    if( (client_socket = accept(socket, (struct sockaddr *) &client,
                                  &client_len)) < 0)
    {
      printf("Error Connecting to Client");
    }
    
    process( client_socket );
    
  }//end while that waits for clients
  
}//end listen

void process( int socket )
{
  printf("Processing client request \n");
  char msg_c[BUF];
    //buffer that will hold client msg
  int msg_size;

  if( (msg_size = recv( socket, msg_c, BUF, 0)) < 0)
  {
    printf("Error getting msg from client \n");
  }

  printf("Msg: %s", msg_c);
  close(socket);
  exit(0);
}//end process
