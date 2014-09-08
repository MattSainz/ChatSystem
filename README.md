ChatSystem
==========

Programming Assignment One for CSCI 4273 Network Systems 

Specifications:
  Start s_name: Start a new chat session s_name is the name of the individual you would like to chat with 

  Find s_name: Find the persion you want to chat with

  Terminate s_name: Ends a chat session

  Starts by printing its UDP port number to standard output

  On receiving start create and binds a TCP socket and creates a new process server
    Any errors return -1 

  on receiving a terminate ommand, the chat coordinator terminates the session 
   A chat session can be terminated only by it's session server 

Session Server Specification 
  Submit message_length message - max message lenght is 80 chars 

  GetNext - get the next message that is not yet read by this client from the chat session 

  GetAll - Get all the messages that are not yet read by this client 

  Leave - leave the chat session 

  server waits for new chat clients to connect by using listen() 
  for each client that connects the session server creates a seprate TCP socket (auto done by accept)

  matains a chat history 

  recive Sumbit - stores message in chat history 

  recive GetNext - returns the chat message in its chat history following the last chat message read by that client and updates the 
  idenity of the last message read by this client 

  recive GetAll - return a list of all the new messages and then all of the new messages 

  recive leave - closes TCP connection 

Chat Client Specification 

  Start
  Join 
  Sumbit 
  GetNext
  GetAll
  Leave
  Exit

  starts with two command line parameters hostname and ip of chat server and port number of the chat server 
