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

Connection* new_connection()
{ 
  struct sockaddr_in my_address;
    //holds connection information
  
  Connection *to_ret;
  
  to_ret = (Connection*)malloc(sizeof(Connection));
  
  int my_socket;

  my_socket = create_socket(SOCK_STREAM,IPPROTO_TCP);

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
    Connection* tcp_connection = new_connection();    
      //TODO create session server
    DictInsert(sessions, s_name, tcp_connection->port);
    
    start_new_session(tcp_connection->port, tcp_connection->socket);
    
    if( tcp_connection != NULL ) free(tcp_connection);
    
    to_ret = tcp_connection->port;
    
  }//end check for existing session
  
  return to_ret;
}//end my start

 int my_find(char* s_name)
{
  int to_ret = -1;
    //the session does not already exist
  if( DictSearch(sessions,s_name) != 0 )
  {
    to_ret = DictSearch(sessions,s_name);
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

void error(char *msg) {
  perror(msg);
  exit(1);
}

void run_chat_coordinator()
{
  //run_chat_coordinator();
  int sockfd; /* socket */
  int BUFSIZE = 80;
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  portno = htons(0);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
    else
    printf("port number %d\n", ntohs(sin.sin_port));

  struct sockaddr_in clientaddr; /* client addr */
  clientlen = sizeof(clientaddr);
  
  sessions = DictCreate();
  
  char *command;
  char *arg;
  char to_ret[BUFSIZE];
  char *tmp;
  
  if( DEBUG != 1)
  {
    while(1)
    {
      printf("Waiting For connection \n");
      
      bzero(buf, BUFSIZE);
      bzero(to_ret, BUFSIZE);
      n = recvfrom(sockfd, buf, BUFSIZE, 0,
	(struct sockaddr *) &clientaddr, &clientlen);
      
      printf("Got message \n");
        //wait for client to send a message
      tmp = buf;

      command = strsep(&tmp, ",");
      printf("Command: %s \n", command);

      arg     = strsep(&tmp, ",");
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
        n = sendto(sockfd, to_ret, BUFSIZE, 0, 
            (struct sockaddr *) &clientaddr, clientlen);
      }
      else if(strcmp(command, FIND) == 0)
      {
        sprintf(to_ret,"%d",my_find(arg));
        n = sendto(sockfd, to_ret, strlen(to_ret), 0, 
            (struct sockaddr *) &clientaddr, clientlen);
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
    if( buf != NULL ) free(buf);
  }
  else
  {
    my_start("test");
  }
  
  close(sockfd);
  
}//end run_chat_coordinator

/*
 * Standard main method
 */

int main(int argc, char** argv)
{  
  run_chat_coordinator();
}//end main



