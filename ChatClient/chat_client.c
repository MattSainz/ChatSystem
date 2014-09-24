#include "chat_client.h"


void session_start(char* s_name )
{
  
}//end session_start
  
void session_join(char* s_name )
{
  
}//end session_join

void submit(char* msg){
  
}//end submit

void get_next(){
  
}//end get_next

void get_all(){
  
}//end get_all

void leave(){
  
}//end leave

void session_exit(){
  
}//end session_exit

char* send_command(char* command, char* msg){
  return NULL;
}//end send_command

void runner(int ip, int port)
{
  int input;
  char input_s[MSG_SIZE];
  
  printf("Welcome to the chat system Please enter a command \n");
  printf("1) Get Next, 2) Get All, 3) Start, 4) Submit message, 5) Join,");
  printf("  6) Leave, 7)Exit 8) print this message again \n");
  
  while(1)
  { 
    printf("Command: ");
    scanf("%d", &input);
    
    switch(input)
    {
      case 1:
        get_next();
        break;
      case 2:
        get_all();
        break;
      case 3:
        printf("Enter Session to Start: ");
        scanf("%s", input_s);
        session_start( input_s );
        break;
      case 4:
        printf("Enter a new message: ");
        scanf("%s", input_s);
        submit( input_s );
        break;
      case 5:
        printf("Enter a session to join: ");
        scanf("%s", input_s);
        session_join( input_s );
        break;
      case 6:
        leave();
        break;
      case 7:
        leave();
        printf("Good by \n");
        exit(0);
        break;
      case 8:
        printf("1) Get Next, 2) Get All, 3) Start, 4) Submit message, 5) Join,");
        printf("  6) Leave, 7) print this message again \n");
        break;
      default:
        printf("Error command %d not found \n", input);
        printf("1) Get Next, 2) Get All, 3) Start, 4) Submit message, 5) Join,");
        printf("  6) Leave, 7) print this message again \n");
        break;
    }//end switch
    
  }//end while
  
}//end runner

int main(int argc, char** argv)
{
  if( argc == 3)
  {
    printf("Host Address: %s Host Port: %s \n", argv[1], argv[2]);
    //TODO check for formatting
 
    runner( atoi(argv[1]), atoi(argv[2]) ); 
  }
  else
  {
    printf("Error incorrect number of arguments usage ip port \n");
  } 
  exit(-1);
}//end main
