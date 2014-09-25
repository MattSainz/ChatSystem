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

void my_wait()
{
  printf("Waiting for connections \n");
  struct sockaddr_in client;
  int client_socket;
  int client_len = sizeof(client);
  
  if( listen(tcp_socket, MAXPENDING) < 0 )
  {
    printf("Error too many clients waiting \n");
  }
  
  while(1)
  {
    if( (client_socket = accept(tcp_socket, (struct sockaddr *) &client,
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

  while( msg_size > 0 )
  {

  }
}//end process

int main(int argc, char** argv) 
{
  int to_ret = -1;
  if( argc > 2 )
  {
    tcp_port   = atoi( argv[1] );
    tcp_socket = atoi( argv[2] );
    printf("Running Server \n");
    printf("Port: %d Socket: %d \n", tcp_port, tcp_socket);
    //my_wait();
  }
  return to_ret;
}//end main