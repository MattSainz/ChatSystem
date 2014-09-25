#include "chat_client.h"

void error(char *msg) {
    perror(msg);
    exit(0);
}

char* send_command_server(char* command, char* msg, char*host, char* port)
{
   
   int sockfd,n;
   struct sockaddr_in serveraddr,cliaddr;
   char sendline[1000];
   char recvline[1000];
   struct hostent *server;

   sockfd=socket(AF_INET,SOCK_STREAM,0);

   server = gethostbyname(host);
   if (server == NULL) {
     fprintf(stderr,"ERROR, no such host as %s\n", host);
     exit(0);
   }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
    (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(atoi(port));

   connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

   int msg_len = sizeof(command)+sizeof(msg)+sizeof(char)*2;
   char to_server[msg_len];
   bzero(to_server, msg_len);
   snprintf(to_server, sizeof(to_server), "%s,%s", command, msg); 
    
   sendto(sockfd,to_server,strlen(to_server),0,
          (struct sockaddr *)&serveraddr,sizeof(serveraddr));
    
   n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   recvline[n]=0;
   printf("Got %s: \n", recvline);
   close(sockfd);
  
}

char* send_command_coord(char* command, char* msg, char* host, char* port){
  //connect_sock(host, port, 1);
  int sockfd, portno, n;
  int serverlen;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;
  char buf[80];
 
  hostname = host;
  portno = atoi(port);

  /* socket: create the socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
      error("ERROR opening socket");

  /* gethostbyname: get the server's DNS entry */
  server = gethostbyname(hostname);
  if (server == NULL) {
      fprintf(stderr,"ERROR, no such host as %s\n", hostname);
      exit(0);
  }

    /* build the server's Internet address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
     (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
  
  int msg_len = sizeof(command)+sizeof(msg)+sizeof(char)*2;
  char to_server[msg_len];
  bzero(to_server, msg_len);
  snprintf(to_server, sizeof(to_server), "%s,%s", command, msg);
  
  /* send the message to the server */
  serverlen = sizeof(serveraddr);
  n = sendto(sockfd, to_server, strlen(to_server), 0, &serveraddr, serverlen);
  if (n < 0) 
    error("ERROR in sendto");
    
  /* print the server's reply */
  bzero(buf,80);
  n = recvfrom(sockfd, buf, 80, 0, &serveraddr, &serverlen);
  if (n < 0) 
    error("ERROR in recvfrom");
  
  printf("From server: %s \n", buf);
  
  close(sockfd);
  
  char* to_ret = (char*)malloc(sizeof(to_ret));
  sprintf(to_ret,"%s", buf);
  return to_ret;
}//end send_command

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

void session_start(char* s_name, char* host, char* port )
{
    char* reply = send_command_coord(START, s_name, host, port);
    if(strcmp(reply,"-1") == 0)
    {
      printf("Session %s already exists, cannot start \n", s_name);
    }
    else
    {
      printf("Session %s created...attempting to join \n", s_name);
      session_join(s_name, host, reply);
    }
    free(reply);
}//end session_start

void session_join(char* s_name, char* host, char* port )
{
   printf("Joining %s \n", port);
   char* tmp = "hello server";
   send_command_server(JOIN,tmp,host,port);
}//end session_join

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
    
    return 0;
  }
  else
  {
    printf("Error incorrect number of arguments usage ip port \n");
  } 
  exit(-1);
}//end main
