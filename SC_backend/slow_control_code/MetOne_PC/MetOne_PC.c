/* Program for reading the MetOne particle counter */
/* and putting said readings into a mysql database. */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2017 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "serial.h"

#define INSTNAME "MetOne_PC"

int inst_dev;

int comm_type;

//////////////////////////////////////////////////////////////////////////////////////////

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)  
{
  struct termios       tbuf;  /* serial line settings */
   
  if (strncmp(i_s->dev_type, "serial", 6) == 0) 
    {
      comm_type = TYPE_SERIAL;
      if (( inst_dev = open(i_s->dev_address, (O_RDWR | O_NDELAY), 0)) < 0 ) 
	{
	  fprintf(stderr, "Unable to open tty port specified: %s \n", i_s->dev_address);
	  my_signal = SIGTERM;
	  return(1);
	}
	
      /* set up the serial line parameters :  */
      tbuf.c_cflag = CS8|CREAD|B9600|CLOCAL;
      tbuf.c_iflag = IGNBRK;
      tbuf.c_oflag = 0;
      tbuf.c_lflag = 0;
      tbuf.c_cc[VMIN] = 0;
      tbuf.c_cc[VTIME]= 0; 
	
      if (tcsetattr(inst_dev, TCSANOW, &tbuf) < 0) {
	fprintf(stderr, "Unable to set device '%s' parameters\n", i_s->dev_address);
	my_signal = SIGTERM;
	return(1);
      }
    }
  else
    {
      fprintf(stderr, "Device type must be serial or eth. \n");
      my_signal = SIGTERM;
      return(1);	
    }
    
  return(0);
}

#define _def_clean_up_inst
void clean_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)
{
  close(inst_dev);
}

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  int        query_status;

  s_s->data_type = DONT_AVERAGE_DATA_OR_INSERT;

  
  
  sprintf(cmd_string, "U01\nS");   // start counting
  query_status = query_serial(inst_dev, cmd_string,  strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  sleep(20);
   
  sprintf(cmd_string, "U01\nE");   // stop counting
  query_status = query_serial(inst_dev, cmd_string,  strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  sleep(5);
  
  sprintf(cmd_string, "U01\nL");   // list output
  query_status = query_serial(inst_dev, cmd_string,  strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

  fprintf(stdout, "Return string:\n %s \n", ret_string);

  if(sscanf(ret_string, "%d", &return_int) != 1)
    {
      fprintf(stderr, "Bad return string: \"%s\" in read sensor!\n", ret_string);
      return(1);
    }

  if (query_status != 0)
    {
      fprintf(stderr, "One of the queries failed.\n");
      return(-1);
    }

       
  *val_out = (double)return_int;


  add_val_sensor_struct(s_s, time(NULL), *val_out);
  s_s->rate = 0;
  write_temporary_sensor_data(s_s);
  return(insert_mysql_sensor_data(s_s->name, s_s->times[s_s->index], s_s->vals[s_s->index], s_s->rate));
      
}

	 
#include "main.h"
