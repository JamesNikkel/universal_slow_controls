/* Program for controlling the PROSPECT belt driven source deployment system*/
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2017 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "SRC_Control_test"

int number_of_loops = 2;
double max_extension = 300;



void do_loop(struct inst_struct *i_s ,struct sensor_struct *s_s)
{
  int i;
  double current_position =  max_extension;
  for (i = 0; i < number_of_loops; i++)
    {
      insert_mysql_sensor_data(i_s->user1, time(NULL), current_position, 0.0);
      sleep(60);
      current_position =- 50;
      if  (current_position < 50)
	current_position = max_extension;
    }
}

#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
 
 
  if (strncmp(s_s->subtype, "RUN", 3) == 0)  // Start run
    {  
      if (s_s->new_set_val > 0.5) 
	{  
	  insert_mysql_sensor_data(s_s->name, time(NULL), 0.0, 0.0);
	  do_loop(i_s, s_s);
	}
    }
  else if (strncmp(s_s->subtype, "NUM", 3) == 0)  // max number of loops
    {
      if (s_s->new_set_val > 0) 
	{     
	  number_of_loops = (int)s_s->new_set_val;
	}
    }
  else if (strncmp(s_s->subtype, "MAX", 3) == 0)  // maximum extension of belt 
    {
      if (s_s->new_set_val > 0) 
	 {     
	   max_extension = s_s->new_set_val;
	 }
      
    }
  
  return(0);
}

#include "main.h"
