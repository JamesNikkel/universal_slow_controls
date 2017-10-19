
#include <stdio.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>


int inst_dev_1;
int inst_dev_2;

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


int query_tcp(int fd, char *cmd_string, size_t c_count, char *ret_string, size_t r_count)
{
  ssize_t rdstatus;   
  int     select_ret;
  fd_set  rfds;
  struct timeval tv;
  
  bzero(ret_string, r_count);

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



int set_up(void)
{
  if ((inst_dev_1 = connect_tcp_raw("192.168.1.5", 64000)) < 0)
    {
      printf("Connect 1 failed. \n");
      return(1);

    }     
  
  if ((inst_dev_2 = connect_tcp_raw("192.168.1.97", 5000)) < 0)
    {
      printf("Connect 2 failed. \n");
      return(1);
    }
}

double read_z(void);
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "M0%c%c", CR, LF);
	          
  query_tcp(inst_dev_1, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "M0,%d", &return_int) != 1)
    {
      printf("Bad return string: \"%s\" in read z!\n", ret_string);
      return(-1);
    }
      
  return((double)return_int/1000.0);
}

double read_x(void);
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "%d R 0\n", 2);

  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &return_int) != 1)
    {
      printf("Bad return string: \"%s\" in read x!\n", ret_string);
      return(-2);
    }
  return((double)return_int/10.0);
}

void home_x(void)
{
  sprintf(cmd_string, "%d H 0\n", 2, (int)(10*dx));
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}


void goto_x(double target_x);
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "%d G %d\n", 2, (int)(10*target_x));

  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}


void extend_x(double dx);
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "%d E %d\n", 2, (int)(10*dx));

  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}

void scan(double X1, double X2, double dX)
{
  double current_x;
  double target_x;
  target_x = x1;
  
  goto_x(target_x);

  while((abs(read_x() - X1)) > dX)
    {
      msleep(100);
    }
  do
    {
      target_x+=dX;

      goto_x(target_x);

      while ((current_x=read_x()) < 0)
	{
	  msleep(10);
	}
      fprintf(stdout, "%lf, %lf \n", current_x, read_z);
    } while (abs(current_x-X2) > dX);
  
}

int main (int argc, char *argv[])
{
  double XXX, X1, X2, dX;
  
  if (set_up())
    exit(1);
  
  if (argc > 1)
    {
      if ((strncasecmp(argv[1], "help", 4) == 0) || (strncasecmp(argv[1], "-h", 2) == 0) || (strncasecmp(argv[1], "--h", 3) == 0))
	{
	  fprintf(stdout, "Usage: %s home            to 'home' the drive.\n", argv[0]);
	  fprintf(stdout, "   or: %s goto XXX        to move the sensor to XXX(cm) \n", argv[0]);
	  fprintf(stdout, "   or: %s scan X1 X2 dX   to scan from X1 to X2 in steps of dX (cm) \n", argv[0]);
	  exit(1);
	}
      else if (strncasecmp(argv[1], "home", 4) == 0)
	{
	  if (set_up())
	    exit(1);
	  home_x();
	  close(inst_dev_1);
	  close(inst_dev_2);
	  exit(1);
	}
      else if ((strncasecmp(argv[1], "goto", 4) == 0) && (argc > 2))
	if (sscanf(argv[2], &XXX) == 1)
	  {
	    if (set_up())
	      exit(1);
	    goto_x(XXX);
	    close(inst_dev_1);
	    close(inst_dev_2);
	    exit(1);
	  }
	else
	  {
	    fprintf(stdout, "Bad target value in 'goto'. \n");
	    exit(1);
	  }
      else if  ((strncasecmp(argv[1], "scan", 4) == 0) && (argc > 5))
	if ((sscanf(argv[2], &X1) == 1) && (sscanf(argv[2], &X2) == 1) && (sscanf(argv[2], &dX) == 1))
	  {
	    if (set_up())
	      exit(1);
	    scan(X1, X2, dX);
	    close(inst_dev_1);
	    close(inst_dev_2);
	    exit(1);
	  }
	else
	  {
	    fprintf(stdout, "Bad values in 'scan'. \n");
	    exit(1);
	  }
      else
	{
	    fprintf(stdout, "Bad command.  Try --help \n");
	    exit(1);
	}
    }
}
