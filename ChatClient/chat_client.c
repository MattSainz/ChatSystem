#include "chat_client.h"


void session_start(char* s_name, char* host, char* port )
{
  //connect_sock(host, port, 1);
  struct hostent  *phe;   /* pointer to host information entry    */
  int     s;              /* socket descriptor                    */

  struct sockaddr_in foo;
  memset(&foo, 0, sizeof(foo));
  foo.sin_family = AF_INET;
  foo.sin_port   = htons(32453);
 // if ((foo.sin_port=htons(32453) == 0))
   //       errexit("can't get \"%s\" port number\n", port);

  if ( (phe = gethostbyname(host)) )
    memcpy(&foo.sin_addr, phe->h_addr, phe->h_length);
  else if ( (foo.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
    errexit("can't get \"%s\" host entry\n", host);

    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    udp_socket = s;
  
  if (s < 0)
  {
    errexit("can't create socket: %s\n", strerror(errno));
    tcp_socket = -1;
    udp_socket = -1;
  }
  printf("Addr: %d \n", foo.sin_port);
  
  //if (connect(s, (struct sockaddr *)&foo, sizeof(foo)) < 0)
    //      errexit("can't connect to %s.%s: %s\n", host, port,
      //            strerror(errno));
  
  int len = sizeof(struct sockaddr_in);
  
  int msg_len = sizeof(s_name)+sizeof(START)+sizeof(char)*2;
  char to_server[msg_len];
  snprintf(to_server, sizeof(to_server), "%s,%s", START, s_name);
  
  printf("Addr: %d\n", foo.sin_port);
  
  if (sendto(s, "test", sizeof("test"), 0, (struct sockaddr *) &foo,
                len) == -1)
  {
    printf("Bad \n");
  }
  
  printf("In here \n");
  
  int n;
  char buf[MSG_SIZE];
  bzero(buf,MSG_SIZE);
  
  while ( (n = recvfrom(udp_socket, buf, MSG_SIZE, 0, (struct sockaddr *) &foo,
           &len)) != -1)
  {
    printf("From ClientCoord: ");
    fputs(buf, stdout);
  }
  printf("Stuck \n");
}//end session_start
  
void session_join(char* s_name, char* host, char* port )
{
  
}//end session_join

void submit(char* msg){
  
}//end submit

void get_next(){
  
}//end get_next

void get_all(){
  
}//end get_all

void leave(){
  close(tcp_socket);
  close(udp_socket);
}//end leave

void session_exit(){
  
}//end session_exit

char* send_command(char* command, char* msg){
  return NULL;
}//end send_command

int errexit(const char *format, ...)
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
  int     s;              /* socket descriptor                    */


  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  if ((addr.sin_port=htons((unsigned short)atoi(portnum))) == 0)
          errexit("can't get \"%s\" port number\n", portnum);

  if ( (phe = gethostbyname(host)) )
          memcpy(&addr.sin_addr, phe->h_addr, phe->h_length);
  else if ( (addr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
          errexit("can't get \"%s\" host entry\n", host);

  if( 0 == con_type )
  {  
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    tcp_socket = s;
  }
  else
  {
    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    udp_socket = s;
  }
  
  if (s < 0)
  {
    errexit("can't create socket: %s\n", strerror(errno));
    tcp_socket = -1;
    udp_socket = -1;
  }
  printf("Addr: %d \n", addr.sin_port);
  
  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
          errexit("can't connect to %s.%s: %s\n", host, portnum,
                  strerror(errno));
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
        session_start( input_s, ip, port );
        break;
      case 4:
        printf("Enter a new message: ");
        scanf("%s", input_s);
        submit( input_s );
        break;
      case 5:
        printf("Enter a session to join: ");
        scanf("%s", input_s);
        session_join( input_s, ip, port );
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
