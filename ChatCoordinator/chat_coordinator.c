/* 
 * File:   chat_coordinator.c
 * Author: Matthias
 *
 * Created on September 15, 2014, 3:13 PM
 */

#include "chat_coordinator.h"

Dict sessions;
int
errexit(const char *format, ...)
{
    fprintf("error: %s",format);
        exit(1);
}

Connection* new_connection()
{ 
  Connection *to_ret;
  int portnum = 0;
  to_ret = (Connection*)malloc(sizeof(Connection));
  
    struct sockaddr_in sin; /* an Internet endpoint address  */
    int     s;              /* socket descriptor             */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

/* Allocate a socket */
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0)
        errexit("can't create socket: %s\n", strerror(errno));

/* Bind the socket */
  
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        errexit("can't bind: %s\n", strerror(errno));
    }
    else 
    {
        int socklen = sizeof(sin);

        if (getsockname(s, (struct sockaddr *)&sin, &socklen) < 0)
                errexit("getsockname: %s\n", strerror(errno));
        printf("New server port number is %d\n", ntohs(sin.sin_port));
    }
 
    if (listen(s, 5) < 0)
        errexit("can't listen on %s port: %s\n", portnum, strerror(errno));
    to_ret->port = sin.sin_port;
    to_ret->socket = s;
  return to_ret;
}//end bind socket

void start_new_session(int port , int socket)
{
  int pid;

  if( (pid = fork()) == 0)
  {
      printf("Port: %d Socket: %d \n", port, socket);
      my_wait(socket);

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



