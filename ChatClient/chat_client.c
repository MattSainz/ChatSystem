#include "chat_client.h"


void session_start(char* s_name );
  
void session_join(char* s_name );

void submit(char* msg);

void get_next();

void get_all();

void leave();

void session_exit();

char* send_command(char* command, char* msg);

void runner(int ip, int port)
{
  
}//end runner

int main(int argc, char** argv)
{
  if( argc == 3)
  {
    printf("Host Address: %s Host Port: %s \n", argv[1], argv[2]);
    //TODO check for formatting
    runner(argv[1], argv[2]); 
  }
  else
  {
    printf("Error incorrect number of arguments usage ip port \n");
  }
  
  exit(-1);
}//end main
