/* Program for reading out the Edwards TIC pump/pressure controller (D397-00-000)*/
/* using serial over ethernet */
/* and putting said readings in to a mysql database. */
/* James Nikkel */
/* james.nikkel@rhul.ac.uk */
/* Copyright 2012 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "Edwards_TIC"

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

  char       cmd_string[16];
  char       ret_string[256];                      
  int        v1, v2;
  
  if (strncmp(s_s->subtype, "Pressure", 3) == 0)  // Read out value for pressure gauge 1, 2, or 3.
    {
      if (s_s->num == 1)
	sprintf(cmd_string, "?V913%c", CR);
      else if (s_s->num == 2)
	sprintf(cmd_string, "?V914%c", CR);
      else if (s_s->num == 3)
	sprintf(cmd_string, "?V915%c", CR);
      else
	{
	  fprintf(stderr, "Gauge number must be 1, 2, or 3, not %d.\n", s_s->num);
	  return(1);
	}
      
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(300);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  
      if(sscanf(ret_string, "=%*s %lf;%*s", val_out) != 1)   // This is in Pascals!  Crazy.  I know, right?
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read_sensor!\n", ret_string);
	  return(1);
	}
      *val_out *= 0.01;  // Convert to mBar
      
    }
  else if (strncmp(s_s->subtype, "Pump", 3) == 0)  // Read out pump status for turbo or backing.
    {
      if (s_s->num < 1 || s_s->num > 2) // Check correct pump number
	{
	  fprintf(stderr, "Pump number must be 1 (for turbo) or 2 (for backing), not %d.\n", s_s->num);
	  return(1);
	}
      
      sprintf(cmd_string, "?V902%c", CR);

      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(300);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      
      if(sscanf(ret_string, "=%*s %d;%d;%*s", &v1, &v2) != 2)   // 
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read_sensor!\n", ret_string);
	  return(1);
	}
      if (s_s->num == 1)
	*val_out = (double)v1;
      else
	*val_out = (double)v2;
    }
  else
    // Print an error if invalid subtype is entered
    {
      fprintf(stderr, "Wrong type for %s \n", s_s->name);
      return(1);
    }
  
  
  msleep(1000);
  return(0);
}


#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
  char       cmd_string[64];
  char       ret_string[64];
  int        ret_status;

  if (strncmp(s_s->subtype, "Pump", 3) == 0)  // Set the control loop setpoint
    {
      if (s_s->num < 1 || s_s->num > 2) // Check correct pump number
	{
	  fprintf(stderr, "Pump number must be 1 (for turbo) or 2 (for backing), not %d.\n", s_s->num);
	  return(1);
	}
      
      if (s_s->num == 1)  //Turbo
	{
	  if (s_s->new_set_val > 0.5)   
	    sprintf(cmd_string, "!C904 1%c", CR);   // On
	  else
	    sprintf(cmd_string, "!C904 0%c", CR);   // Off
	}
      else        //  Backing
 	{
	  if (s_s->new_set_val > 0.5)   
	    sprintf(cmd_string, "!C910 1%c", CR);   // On
	  else
	    sprintf(cmd_string, "!C910 0%c", CR);   // Off
	}
      
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(1000);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

      if(sscanf(ret_string, "*%*s %d", &ret_status) != 1)   // 
	{
	  fprintf(stderr, "Bad return string: \"%s\" in set_sensor!\n", ret_string);
	  return(1);
	}

      if (s_s->new_set_val > 0.5) 
	{
	  if (ret_status != 1)
	    fprintf(stderr, "Trouble turning on pump! (%s)\n", s_s->name);
	}
      else
	{
	  if (ret_status != 0)
	    fprintf(stderr, "Trouble turning off pump! (%s)\n", s_s->name);
	}	
      
    }
  
  else       // Print an error if invalid subtype is entered
    {
      fprintf(stderr, "Wrong type for %s \n", s_s->name);
      return(1);
    } 

  return(0);
}



#include "main.h"
