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

int
errexit(const char *format, ...)
{
        va_list args;

        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        exit(1);
}

void connect_sock(const char* host, const char* portnum, int con_type)
{
  struct hostent  *phe;   /* pointer to host information entry    */
  struct sockaddr_in sin; /* an Internet endpoint address         */
  int     s;              /* socket descriptor                    */


  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;

  if ((sin.sin_port=htons((unsigned short)atoi(portnum))) == 0)
          errexit("can't get \"%s\" port number\n", portnum);

  if ( phe = gethostbyname(host) )
          memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
  else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
          errexit("can't get \"%s\" host entry\n", host);

  if( 0 == con_type )
  {  
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  else
  {
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);
  }
  
  if (s < 0)
          errexit("can't create socket: %s\n", strerror(errno));

  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
          errexit("can't connect to %s.%s: %s\n", host, portnum,
                  strerror(errno));
  return s;
}//end connect_sock

void runner(char* ip, char* port)
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
 
    runner( argv[1], argv[2] ); 
  }
  else
  {
    printf("Error incorrect number of arguments usage ip port \n");
  } 
  exit(-1);
}//end main
