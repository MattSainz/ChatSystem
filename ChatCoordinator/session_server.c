/*
 * Session Server by Matthias Sainz
 */
#include "session_server.h"

int msg_in_history(char* id, int msg )
{
  int to_ret = 0;
  char *history = DictSearch( msg_history, id ); 
  char* old_msg;
  char id_s[20];

  sprintf(id_s,"%d",id);

  while( (old_msg = strsep(&history,",") != NULL && old_msg != '\0'))
  {
    if(strcmp(id_s,old_msg) == 0)
    {
      to_ret = 1;
      break;
    }
  }
  return to_ret;
}

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

void join(int socket)
{
  char new_client_id[20];
  sprintf(new_client_id,"%d", client_id++);
  DictInsert(clients, new_client_id, -1);
  write(socket, new_client_id, sizeof(new_client_id));
}

void submit(char *id, char* msg, int socket )
{
  char to_ret[8];
  if( strlen(msg) < 80)
  {
    DictInsertS(messages, num_msg, msg); 
    history_add(id);
    num_msg++;
    sprintf(to_ret,"%d", 1);
  }
  else
  {
    sprintf(to_ret,"%s","Message length must be less than 80 chars");
  }
  write(socket, to_ret, sizeof(to_ret)); 
}//end submit

void get_next(char* client, int socket)
{
  int last_msg  = DictSearch( clients, client );
  int new_msg = last_msg + 1;
  char to_ret[MSG_SIZE];
  printf(" Client: %s Last_Msg: %d New_Msg: %d \n", client, last_msg, new_msg);
  if( msg_in_history(client, last_msg) == 0 ) 
  {
    if( new_msg < num_msg)
    {
      sprintf(to_ret,"%s", DictSearchS(messages, new_msg)); 
      DictDelete(clients, client);
      DictInsert(clients, client, new_msg);
    }
    else
    {
      sprintf(to_ret,"%s","No new messages ");
    }
  }

  write(socket, to_ret, sizeof(to_ret)); 
}//end get_next

void get_all(char* client, int socket)
{
    
}//end get_all

void leave(char* clinet, int socket)
{
    
}//end leave

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

  char to_ret[sizeof(char)*BUFSIZE];

  switch(command_int)
  {
    case 4:
      submit(id, arg, socket );
      break;
    case 1:
      get_next( id, socket );
      break;
    case 2:
      get_all( id, socket );
      break;
    case 7:
      leave( id, socket );
      break;
    case 5:
      join(socket);
      break;
    default:
      sprintf(to_ret,"%s", " Command not Understood");
      break;
  }//end switch

    printf("Closed socket \n");
  }
  return msg_size;
}//end process

void init()
{
  num_msg     = 0;
  client_id   = 0;
  messages    = DictCreateS();
  clients     = DictCreate();
  msg_history = DictCreateS();
}//end init
