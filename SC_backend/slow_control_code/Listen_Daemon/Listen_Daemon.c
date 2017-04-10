/* Listens on a port and takes data from connecting devices and dumps them to */
/* the DB. */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2016 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"
 
#define INSTNAME "Listen_Daemon"


int setup(struct inst_struct *i_s)
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, 
		     (struct sockaddr *) &cli_addr, 
		     &clilen);
  if (newsockfd < 0) 
    error("ERROR on accept");
  bzero(buffer,256);
  n = read(newsockfd,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);
  n = write(newsockfd,"I got your message",18);
  if (n < 0) error("ERROR writing to socket");
  close(newsockfd);
  close(sockfd);
  return(0);
}

void listen_loop(struct inst_struct *i_s)
{

  
  return;
}



int main (int argc, char *argv[])
{
  char                   **my_argv;
  char                   inst_name[16];
  int                    i;
  struct inst_struct     this_inst;
  struct sensor_struct   *all_sensors;

  // save restart arguments
  my_argv = malloc(sizeof(char *) * (argc + 1));
  for (i = 0; i < argc; i++) 
    my_argv[i] = strdup(argv[i]);
   
  my_argv[i] = NULL;   
    
  sprintf(db_conf_file, DEF_DB_CONF_FILE);

  parse_CL_for_string(argc,  argv, INSTNAME, inst_name);
  read_mysql_inst_struct(&this_inst, inst_name);
    
  // detach current process
  daemonize(this_inst.name);
    
  my_signal = 0;
  // ignore these signals 
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);    
  // install signal handler
  signal(SIGHUP, handler);
  signal(SIGINT, handler);
  signal(SIGQUIT, handler);
  signal(SIGTERM, handler);

  register_inst(&this_inst);

  if (set_up_inst(&this_inst, all_sensors))
    {
      msleep(1000);
      my_signal = SIGHUP;
    }
  mysql_inst_run_status(&this_inst);
   
  while (my_signal == 0)     //  main loop here!
    {
      listen_loop(&this_inst);
      mysql_inst_run_status(&this_inst);
    }
    
  /////////////  Clean up if we get a signal
  clean_up_inst(&this_inst, all_sensors);

  unregister_inst(&this_inst);
  free(all_sensors);

  if (my_signal == SIGHUP)         ///  restart called
    {
      long fd;
      // close all files before restart
      for (fd = sysconf(_SC_OPEN_MAX); fd > 2; fd--) 
	{
	  int flag;
	  if ((flag = fcntl(fd, F_GETFD, 0)) != -1)
	    fcntl(fd, F_SETFD, flag | FD_CLOEXEC);
	}
      sleep(2);
      execv(my_argv[0], my_argv);
      fprintf(stderr, "execv() failed.");
      exit(1);
    }

  for (i = 0; my_argv[i] != NULL; i++)
    free(my_argv[i]);
    
  free(my_argv);

  exit(0);
}


