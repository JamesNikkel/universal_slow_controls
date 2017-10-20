
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <errno.h>

#include <string.h>
#include <strings.h>

int inst_dev_1;
int inst_dev_2;

static char CR   __attribute__ ((unused)) = 0x0D;
static char LF   __attribute__ ((unused)) = 0x0A;

void msleep(double sleep_time)
{
  usleep((int)(sleep_time*1e3));
}

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
      exit(1);
    }
  
    // Set the TCP no delay flag
    // SOL_TCP = IPPROTO_TCP 
  option = 1;
  if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int)) < 0)
    {
      fprintf(stderr, "Could not set option: %d \n", option);
      close(fd);
      exit(1);
    }
  
  // Set the IP low delay option
  option = IPTOS_LOWDELAY;
  if (setsockopt(fd, IPPROTO_TCP, IP_TOS, (const void *)&option, sizeof(int)) < 0)
    {
      fprintf(stderr, "Could not set option: %d \n", option);
      close(fd);
      exit(1);
    }
  
  if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0 )
    {
      fprintf(stderr, "Could not make connection to: %s:%d \n", IP_address, port);
      close(fd);
      exit(1);
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
  tv.tv_sec = 5;
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



void set_up(void)
{
  if ((inst_dev_1 = connect_tcp_raw("192.168.1.5", 64000)) < 0)
    {
      printf("Connect 1 failed. \n");
      exit(1);
    }     
  
  if ((inst_dev_2 = connect_tcp_raw("192.168.1.97", 5000)) < 0)
    {
      printf("Connect 2 failed. \n");
      exit(1);
    }
}


void clean_up(void)
{
   close(inst_dev_1);
   close(inst_dev_2);
   exit(1);
}


int read_z(double *z_val)
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "M0%c%c", CR, LF);
	          
  query_tcp(inst_dev_1, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "M0,%d", &return_int) != 1)
    {
      fprintf(stdout, "Bad return string: \"%s\" in read_z!\n", ret_string);
      return(-1);
    }

  *z_val = (double)return_int/1000.0;
  return(0);
}

int read_x(double *x_val)
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "%d R 0\n", 2);

  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if (ret_string[0] == '-')
    return(-1);

  if (sscanf(ret_string, "%d", &return_int) != 1)
    {
      fprintf(stdout, "Bad return string: \"%s\" in read_x!\n", ret_string);
      return(-1);
    }
  if ((return_int == 1) ||  (return_int == -1))
    return(-1);
  
  *x_val = (double)return_int/100.0;
  
  return(0);
}

void home_x(void)
{
  char       cmd_string[64];
  sprintf(cmd_string, "%d H 0\n", 2);
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}

void halt_x(void)
{
  char       cmd_string[64];
  sprintf(cmd_string, "%d X 0\n", 2);
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}


void goto_x(double target_x)
{
  char       cmd_string[64]; 
  sprintf(cmd_string, "%d G %d\n", 2, (int)(10*target_x));
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}

unsigned long read_counter(void)
{
  unsigned long  counts;
  char       cmd_string[64]; 
  char       ret_string[64];             

  sprintf(cmd_string, "%d C 0\n", 2);

  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

  if (sscanf(ret_string, "%lu", &counts) !=1) 
    {
      fprintf(stderr, "Bad return string: \"%s\" \n", ret_string);
      return(1);
    }
  return(counts);
}

void reset_counter(void)
{
  char       cmd_string[64];
  sprintf(cmd_string, "%d I 0\n", 2);
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}

void scan(double X1, double X2, double dX)
{
  double x_val, z_val;
  double current_x = -1;
  double target_x;
  int i;
  
  goto_x(X1);

  while (read_x(&x_val) == -1)
    {
      msleep(100);
    }
  
  reset_counter();

  goto_x(X2);
  while (read_x(&x_val) == -1)
    {
      read_z(&z_val);
      current_x = (double)read_counter() * 0.000625 + X1;
      fprintf(stdout, "%lf, %lf \n", current_x, z_val);
      msleep(100);
    }
}

int main (int argc, char *argv[])
{
  double XXX, X1, X2, dX;
  double x_val, z_val;
  
  if (argc > 1)
    {
      if ((strncasecmp(argv[1], "help", 4) == 0) || (strncasecmp(argv[1], "-h", 2) == 0) || (strncasecmp(argv[1], "--h", 3) == 0))
	{
	  fprintf(stdout, "Usage: %s home            to 'home' the drive.\n", argv[0]);
	  fprintf(stdout, "   or: %s halt            to halt motion. \n", argv[0]);
	  fprintf(stdout, "   or: %s read            to read out the x and z values. \n", argv[0]);
	  fprintf(stdout, "   or: %s goto XXX        to move the sensor to XXX(cm) \n", argv[0]);
	  fprintf(stdout, "   or: %s scan X1 X2 dX   to scan from X1 to X2 in steps of dX (cm) \n", argv[0]);
	  exit(1);
	}
      else if (strncasecmp(argv[1], "home", 4) == 0)
	{
	  set_up();
	  home_x();
	  clean_up();
	}
       else if (strncasecmp(argv[1], "halt", 4) == 0)
	{
	  set_up();
	  halt_x();
	  clean_up();
	}
      else if (strncasecmp(argv[1], "read", 4) == 0)
	{
	  set_up();
	  read_x(&x_val);
	  read_z(&z_val);

	  fprintf(stdout, "Current X position: %lf (cm).\n", x_val);
	  fprintf(stdout, "Current Z position: %lf (mm).\n", z_val);
	  fprintf(stdout, "Current counts:     %lu  .\n", read_counter());
	  
	  clean_up();
	}
      else if ((strncasecmp(argv[1], "goto", 4) == 0) && (argc > 2))
	{
	  if (sscanf(argv[2], "%lf", &XXX) == 1)
	    {
	      set_up();
	      goto_x(XXX);
	      clean_up();
	    }
	  else
	    {
	      fprintf(stdout, "Bad target value in 'goto'. \n");
	      exit(1);
	    }
	}
      else if  ((strncasecmp(argv[1], "scan", 4) == 0) && (argc > 4))
	{
	  if ((sscanf(argv[2], "%lf", &X1) == 1) && (sscanf(argv[3], "%lf", &X2) == 1) && (sscanf(argv[4], "%lf", &dX) == 1))
	    {
	      set_up();
	      scan(X1, X2, dX);
	      clean_up();
	    }
	  else
	    {
	      fprintf(stdout, "Bad values in 'scan'. \n");
	      exit(1);
	    }
	}
      else
	{
	  fprintf(stdout, "Bad command.  Try --help \n");
	  exit(1);
	}
    }
}
