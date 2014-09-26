/*
 * Session Server by Matthias Sainz
 */
#include "session_server.h"

void history_add(char *id)
{
  int id_i = atoi(id);
  char msg_count[20];
  sprintf(msg_count, "%d", num_msg);

  char* history = DictSearchS( msg_history, id_i ); 
  char new_history[sizeof(history)+sizeof(id)+1];

  snprintf(new_history, sizeof(new_history), "%s,%s", history, msg_count);

  printf("New History: %s \n", new_history);

  DictDeleteS( msg_history, id_i ); 
  DictInsertS( msg_history, id_i, msg_history );
}//end history_edit

int join()
{
  return client_id++;
}

void submit(char *id, char* msg)
{
  printf("Id: %s Msg: %s \n", id, msg);
  DictInsertS(messages, num_msg, msg); 
  history_add(id);
  num_msg++;
}//end submit

void get_next(char* client)
{
    
}//end get_next

void get_all(char* client)
{
    
}//end get_all

void leave(char* client)
{
    
}//end leave

void my_send(char *msg, int len, int client)
{
    
}//end send

void my_wait(int socket)
{

  struct sockaddr_in client;
  int client_socket;
  int client_len = sizeof(client);  
  listen(socket, 50); 
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


  char msg_c[BUF];
    //buffer that will hold client msg
  char* tmp;
  char* id;
  char* command;
  char* arg;
  
  int msg_size;

  /* Get message from client */
  if( (msg_size = recv( socket, msg_c, BUF, 0)) < 0)
  {
    printf("Error getting msg from client \n");
  }

  tmp = msg_c;

  id = strsep(&tmp, ",");
  printf("id: %s \n", id);

  command = strsep(&tmp, ",");
  printf("Command: %s \n", command);

  arg     = strsep(&tmp, ",");
  printf("Arg: %s \n", arg);
 
  int command_int = atoi(command);
  int new_id;

  char to_ret[BUFSIZE];

  switch(command_int)
  {
    case 4:
      submit(id, arg );
      break;
    case 1:
      get_next( id );
      break;
    case 2:
      get_all( id );
      break;
    case 7:
      leave( id );
      break;
    case 5:
      new_id = join();
      sprintf(to_ret, "%d", new_id);
      printf("to_ret: %s \n", to_ret);
      send(socket, to_ret, sizeof(to_ret), 0); 
      break;
    default:
      sprintf(to_ret,"%s", " Command not Understood");
      send(socket, to_ret, sizeof(to_ret), 0); 
      break;
  }//end switch
  close(socket);
}//end process

void new_connection()
{ 
  int portnum = 0;
  
  struct sockaddr_in sin; /* an Internet endpoint address  */
  int     s;              /* socket descriptor             */

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;

/* Allocate a socket */
  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

/* Bind the socket */
  
  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
  }
  else 
  {
    int socklen = sizeof(sin);

    if (getsockname(s, (struct sockaddr *)&sin, &socklen) < 0)
    printf("New server port number is %d\n", ntohs(sin.sin_port));
  }
 
  tcp_port   = sin.sin_port;
  tcp_socket = s;
}//end bind socket

char* phone_home(char* msg, char* host, char* port){
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

void init()
{
  messages     = DictCreateS;
  clients      = DictCreateS;
  msg_history  = DictCreateS;
  new_connection();  
}

int main(int argc, char** argv) 
{
  int to_ret = -1;
  if( argc > 2 )
  {
    init();
    tcp_port   = atoi( argv[1] );
    tcp_socket = atoi( argv[2] );
    printf("Port: %d Socket: %d", tcp_port, tcp_socket);
    my_wait(tcp_socket);
  }
  return to_ret;
}//end main
