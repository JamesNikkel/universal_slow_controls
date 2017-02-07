/* Program for controlling the PROSPECT belt driven source deployment system*/
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2017 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "SRC_Control"

int inst_dev;

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  if ((inst_dev = connect_tcp(i_s)) < 0)
    {
      fprintf(stderr, "Connect failed. \n");
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
  // returns the temp (which_sens_type == 1)
  // or the heater power (which_sens_type == 2)

  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;

  
  if (strncmp(s_s->subtype, "R", 1) == 0)  // Read out current source position
    {
      //s_s->data_type = DONT_AVERAGE_DATA;

      sprintf(cmd_string, "R\n");

      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(200);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

      if(sscanf(ret_string, "%d", &return_int) != 1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read sensor!\n", ret_string);
	  return(1);
	}
      
      *val_out = return_int/10.0;	
    }
  msleep(1000);
  return(0);
}


#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
  char       cmd_string[64];
  char       ret_string[64];
 
  if (strncmp(s_s->subtype, "H", 1) == 0)  // Go home
    {  
      if (s_s->new_set_val > 0.5) 
	{  
	  sprintf(cmd_string, "H\n"); 	   
	  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	  insert_mysql_sensor_data(s_s->name, time(NULL), 0.0, 0.0);
	}
    }
  else if (strncmp(s_s->subtype, "G", 1) == 0)  // Goto specified position
    {
      if (s_s->new_set_val > 0) 
	{     
	  sprintf(cmd_string, "G %d\n", (int)(10*s_s->new_set_val));
	  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	}
    }
  else if (strncmp(s_s->subtype, "E", 1) == 0)  // Extent specified amount
    {
      sprintf(cmd_string, "E %d\n", (int)(10*s_s->new_set_val));
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	
    }
  else if (strncmp(s_s->subtype, "S", 1) == 0)  // Extent specified amount
    {
      sprintf(cmd_string, "S %d\n", (int)s_s->new_set_val);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    }

  
  return(0);
}

#include "main.h"
