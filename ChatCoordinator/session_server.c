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
  if( history != 0)
  {
    snprintf(new_history, sizeof(new_history), "%s,%s", history, msg_count);
    DictDeleteS( msg_history, id_i ); 
  }
  else
  {
    sprintf(new_history,"%s",msg_count);
  }

  printf("New History: %s \n", new_history);
  DictInsertS( msg_history, id_i, new_history);
}//end history_edit

int join()
{
  return client_id++;
}

int submit(char *id, char* msg)
{
  printf("Id: %s Msg: %s \n", id, msg);
  DictInsertS(messages, num_msg, msg); 
  history_add(id);
  num_msg++;
  return 0;
  
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

void my_wait(int socket, int port)
{

  init();
  struct sockaddr_in client;
  int client_socket;
  int client_len = sizeof(client);  

  struct sockaddr_in fsin;	/* the from address of a client	*/
  fd_set rfds;			/* read file descriptor set	*/
  fd_set afds;			/* active file descriptor set	*/
  unsigned int	alen;		/* from-address length		*/
  int fd, nfds;

  nfds = getdtablesize();
  FD_ZERO(&afds);
  FD_SET(socket, &afds);

  init();
  
  while(1)
  {
    memcpy(&rfds, &afds, sizeof(rfds));

    if(select(nfds, &rfds, (fd_set *) 0, (fd_set *)0, 
	      (struct timeval *) 0) < 0)
      printf("bad\n");
    if(FD_ISSET(socket, &rfds))
    {
      int ssock;

      alen = sizeof(fsin);
      ssock = accept(socket, (struct sockaddr *) &fsin,
                   &alen); 
	
      FD_SET(ssock, &afds);
    }
    for(fd = 0; fd < nfds; ++fd)
    {
      if(fd != socket && FD_ISSET(fd, &rfds))
      {
	if(process(fd) == 0)
        {
          (void) close(fd);
	  FD_CLR(fd,&afds);
        }
      }
    }
    
  }//end while that waits for clients
  
}//end listen

int process( int socket )
{


  char msg_c[BUF];
    //buffer that will hold client msg
  bzero(msg_c, BUF);
  char* tmp;
  char* id;
  char* command;
  char* arg;
  
  int msg_size;

  /* Get message from client */
  if( (msg_size = read( socket, msg_c, BUF)) < 0)
  {
  }
  else
  {


  tmp = msg_c;

  id = strsep(&tmp, ",");
  printf("id: %s \n", id);

  command = strsep(&tmp, ",");
  printf("Command: %s \n", command);

  arg     = strsep(&tmp, ",");
  printf("Arg: %s \n", arg);
 
  int command_int = atoi(command);
  int new_id;
  int result;

  char to_ret[sizeof(char)*BUFSIZE];

  switch(command_int)
  {
    case 4:
      result = submit(id, arg );
      sprintf(to_ret,"%d",result);
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
      break;
    default:
      sprintf(to_ret,"%s", " Command not Understood");
      break;
  }//end switch

    write(socket, to_ret, sizeof(to_ret)); 
    printf("Closed socket \n");
  }
  return msg_size;
}//end process

void init()
{
  num_msg     = 0;
  client_id   = 0;
  messages    = DictCreateS();
  clients     = DictCreateS();
  msg_history = DictCreateS();
}//end init
