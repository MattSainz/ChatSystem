/* 
 * File:   chat_coordinator.c
 * Author: Matthias
 *
 * Created on September 15, 2014, 3:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

/*
 * creates socket
 */
int create_socket()
{
  int to_ret = 0;
  if ((to_ret = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
  {
    perror("cannot create socket");
    to_ret = -1;
  }
  return 0;
}//end create socket

/*
 * Standard main method
 */
int main(int argc, char** argv)
{  
  return (EXIT_SUCCESS);
}//end main



