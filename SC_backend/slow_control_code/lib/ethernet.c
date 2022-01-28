/* This is a header file for dealing with an ethernet interface. */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2006 */
/* James public licence. */

#include "ethernet.h"

////// Establishes a TCP connection
int connect_tcp_raw(char *IP_address, int port)
{
  int fd;
  int option;
  struct sockaddr_in addr;
    
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(IP_address);
  
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr, "Could not make socket: %d \n", fd);
      my_signal = SIGTERM;
      return(-1);
    }
  
    // Set the TCP no delay flag
    // SOL_TCP = IPPROTO_TCP 
  option = 1;
  if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int)) < 0)
    {
      fprintf(stderr, "Could not set option: %d \n", option);
      close(fd);
      my_signal = SIGTERM;
	return(-1);
    }
  
  // Set the IP low delay option
  option = IPTOS_LOWDELAY;
  if (setsockopt(fd, IPPROTO_TCP, IP_TOS, (const void *)&option, sizeof(int)) < 0)
    {
      fprintf(stderr, "Could not set option: %d \n", option);
      close(fd);
      my_signal = SIGTERM;
      return(-1);
    }
  
  if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0 )
    {
      fprintf(stderr, "Could not make connection to: %s:%d \n", IP_address, port);
      my_signal = SIGTERM;
      close(fd);
      return(-1);
    }
  return(fd);
}

////// Establishes a TCP connection
int connect_tcp(struct inst_struct *i_s)
{
  int port;

  sscanf(i_s->user1, "%d", &port);
  
  return(connect_tcp_raw(i_s->dev_address, port));
}
 

int query_tcp(int fd, char *cmd_string, size_t c_count, char *ret_string, size_t r_count)
{
  ssize_t rdstatus;   
  int     select_ret;
  fd_set  rfds;
  struct timeval tv;
  
  memset(ret_string, 0, r_count);

  if (send(fd, cmd_string, c_count, 0) < 0)
    {
      fprintf(stderr, "send error in query_tcp\n");
      return(1);
    }
  
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = global_tcp_timeout;
  tv.tv_usec = 0;
  
  select_ret = 0;
  while ((select_ret = select(fd+1, &rfds, NULL, NULL, &tv)) == -1)
    { 
      if (errno == EINTR) 
	{					
	  fprintf(stderr, "A non blocked signal was caught.\n");		
	  FD_ZERO(&rfds);						
	  FD_SET(fd, &rfds);				
	} 
      else 
	{							
	  fprintf(stderr, "Select failure.\n"); 
	  return(1);					
	}								
    }								
  
  if (select_ret == 0) // Timeout
    {
      fprintf(stderr, "Timeout in query_tcp.\n");
      return(1);	
    }
  
  while (select_ret) 
    {
      rdstatus = recv(fd, ret_string, r_count, MSG_DONTWAIT);
      if (rdstatus == 0) 
	{
	  fprintf(stderr, "Connection closed.\n");
	  return(1);
	}
      else if (rdstatus < 0) 
	{
	  fprintf(stderr, "Socket failure.\n");
	  return(1);
	}
      select_ret = 0;
    }
  
  if (rdstatus > 0)
    return(0);
  
  if (rdstatus == 0) 
    fprintf(stderr, "Connection closed\n");
  
  return(1);
}


int write_tcp(int fd, char *cmd_string, size_t c_count)
{
  if (send(fd, cmd_string, c_count, 0) < 0)
    {
      fprintf(stderr, "send error in query_tcp\n");
      return(1);
    }
  
  return(0);
}


int read_tcp(int fd, char *ret_string, size_t r_count)
{
  ssize_t rdstatus;   
  int     select_ret;
  fd_set  rfds;
  struct timeval tv;

  memset(ret_string, 0, r_count);

  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = global_tcp_timeout;
  tv.tv_usec = 0;
  
  select_ret = 0;
  while ((select_ret = select(fd+1, &rfds, NULL, NULL, &tv)) == -1)
    { 
      if (errno == EINTR) 
	{					
	  fprintf(stderr, "A non blocked signal was caught.\n");		
	  FD_ZERO(&rfds);						
	  FD_SET(fd, &rfds);				
	} 
      else 
	{							
	  fprintf(stderr, "Select failure.\n"); 
	  return(1);					
	}								
    }								
  
  if (select_ret == 0) // Timeout
    {
      fprintf(stderr, "Timeout in read_tcp.\n");
      return(1);	
    }
  
  while (select_ret) 
    {
      rdstatus = recv(fd, ret_string, r_count, MSG_DONTWAIT);
      if (rdstatus == 0) 
	{
	  fprintf(stderr, "Connection closed.\n");
	  return(1);
	}
      else if (rdstatus < 0) 
	{
	  fprintf(stderr, "Socket failure.\n");
	  return(1);
	}
      select_ret = 0;
    }
  
  if (rdstatus > 0)
    return(0);
  
  if (rdstatus == 0) 
    fprintf(stderr, "Connection closed\n");
  
  return(1);
}

char* mystrcat (char* dest, char* src)
{
  while (*dest) dest++;
  while (*dest++ = *src++);
  return (--dest);
}

int read_long_tcp(int fd, char *ret_string, size_t r_count, int timeout)
{
  ssize_t rdstatus;   
  int     select_ret;
  fd_set  rfds;
  struct timeval tv;
  char chunk[CHUNK_SIZE];
  int size_recv = 0;
  int total_size = 0;
  time_t begin, now;

  char *p = ret_string;

  memset(ret_string, 0, r_count);
  ret_string[0] = '\0';         // initialize the char array to be zero length string


  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  
  select_ret = 0;
  while ((select_ret = select(fd+1, &rfds, NULL, NULL, &tv)) == -1)
    { 
      if (errno == EINTR) 
	{					
	  fprintf(stderr, "A non blocked signal was caught.\n");		
	  FD_ZERO(&rfds);						
	  FD_SET(fd, &rfds);				
	} 
      else 
	{							
	  fprintf(stderr, "Select failure.\n"); 
	  return(1);					
	}								
    }								
  
  if (select_ret == 0) // Timeout
    {
      fprintf(stderr, "Timeout in read_tcp.\n");
      return(1);	
    }
  

  //fcntl(fd, F_SETFL, O_NONBLOCK);

  begin = time(NULL);

  int i = 0;
  while (1) 
    {
      now = time(NULL);
      
      if (((now - begin) > timeout) && ( total_size > 0))
	break;
         
      if ((now - begin) > timeout*2) 
	break;

      memset(chunk, 0, CHUNK_SIZE);
      if ((size_recv = recv(fd, chunk, CHUNK_SIZE-2, MSG_DONTWAIT)) < 0)
	{
	  msleep(50);
	}
      else
	{
	  //chunk[CHUNK_SIZE-1] = '\0';
	  total_size += size_recv;
	  /*
	  if (i < 5)
	    {
	      fprintf(stdout, "i:%d\n", i);
	      fprintf(stdout, "%s\n\n", chunk);
	    }
	  */
	  p = mystrcat(p,chunk);
	  begin = time(NULL);
	}
      i++;
    }
  
  if (total_size > 0)
    return(0);
  
  if (total_size == 0) 
    fprintf(stderr, "Connection closed\n");
  
  return(1);
}
