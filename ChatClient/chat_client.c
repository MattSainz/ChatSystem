#include "chat_client.h"

void error(char *msg) {
  perror(msg);
  exit(0);
}

char* send_command(char* command, char* msg)
{
  /* msg to send to server*/
  char recvline[80];
  char id_s[15];
  int n;
  sprintf(id_s,"%d",id);
  int msg_len = sizeof(id_s)+sizeof(command)+sizeof(msg)+sizeof(char)*2;
  char to_server[msg_len];
  bzero(to_server, msg_len);
  snprintf(to_server, sizeof(to_server), "%s,%s,%s", id_s, command, msg);

  (void) write(tcp_socket ,to_server,strlen(to_server));

  /* Get reply from server */
  bzero((char *) &recvline,80);
  n=read(tcp_socket,recvline,80);
  if( n < 0 )
  {
    printf("Error reading from server \n");
    recvline[0] = '-';
    recvline[1] = '1';
    recvline[2] = '\0';
  }

  char* to_ret = (char*)malloc(sizeof(recvline));
  sprintf(to_ret,"%s", recvline);
  return to_ret;
}

void connect_server(char* host, char* port)
{
    
  /* Vars needed for new tcp connection */
  int sockfd,n;
  struct sockaddr_in serveraddr,cliaddr;
  char sendline[1000];
  struct hostent *server;

  /* Make a socket */
  sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  /* get address of host name */
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
  serveraddr.sin_port = (atoi(port));

  /* connect to server */
  connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  tcp_socket = sockfd;

  }//end send command server

char* send_command_coord(char* command, char* msg ){
  //connect_sock(host, port, 1);
  int sockfd, portno, n;
  int serverlen;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;
  char buf[80];
 
  hostname = coord_host;
  portno = atoi(coord_port);

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
  if( server_port != NULL)
  {
    printf("msg: %s \n", msg);
    char* result = send_command(SUBMIT, msg);
    printf("Submit result: %s \n", result);
    free(result);
  }
  else
  {
    printf("Error you are not connected to a chat server \n");
  }
}//end submit

void get_next(){
  if( server_port != NULL)
  {
    char* next_msg = send_command(GET_NEXT,NULL);
    printf("Message: %s \n", next_msg);
  }
  else
  {
    printf("Error you are not connected to a chat server \n");
  } 
}//end get_next

void get_all(){
  if( server_port != NULL)
  {

  }
  else
  {
    printf("Error you are not connected to a chat server \n");
  }
}//end get_all

void leave(){
  if(server_port != NULL) free(server_port);
}//end leave

void session_exit(){
  
}//end session_exit

void session_start(char* s_name )
{
  char* reply = send_command_coord(START, s_name);
  if(strcmp(reply,"-1") == 0)
  {
    printf("Session %s already exists, cannot start \n", s_name);
    free(reply);
  }
  else
  {
    printf("Session %s created...attempting to join \n", s_name);
    session_join(s_name);
  }
}//end session_start

void session_join(char* s_name)
{
  server_port = send_command_coord(FIND, s_name); 
  connect_server(server_host, server_port);
  char* new_id = send_command(JOIN, s_name);

  if( strcmp(new_id,"-1") != 0 )
  {
    int id_i = atoi(new_id);
    id = id_i;
  }
  else
  {
    printf("Unable to Join Session \n");
  }
  free(new_id);
}//end session_join

void runner()
{
  int input;
  char input_s[sizeof(char)*MSG_SIZE];

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
        scanf(" %[^\n]s", input_s);
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
    id = -1;
    printf("Host Address: %s Host Port: %s \n", argv[1], argv[2]);
    //TODO check for formatting 

    coord_host  = strdup( argv[1] );
    server_host = strdup(coord_host);
    coord_port  = strdup( argv[2] );

    runner(); 
    
    return 0;
  }
  else
  {
    printf("Error incorrect number of arguments usage ip port \n");
  } 
  exit(-1);
}//end main
