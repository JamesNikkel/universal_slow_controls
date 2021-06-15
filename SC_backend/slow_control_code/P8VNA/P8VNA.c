/* Program for reading  */
/* using ethernet */
/* and putting said readings in to a mysql database. */
/* James Nikkel */
/* james.nikkel@rhul.ac.uk */
/* Copyright 2021 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "P8VNA"

int inst_dev;


#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  char       cmd_string[32];
  char       ret_string[64];      

  if ((inst_dev = connect_tcp(i_s)) < 0)
    {
      fprintf(stderr, "Connect failed. \n");
      my_signal = SIGTERM;
      return(1);
    }
  
  msleep(2000);
  read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));
  read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));
  
  
  sprintf(cmd_string, "*CLS%c%c", CR, LF);
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  
  sprintf(cmd_string, "FORM ASCII%c%c", CR, LF);
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  
  read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));
  read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));
  read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));

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

  char       cmd_string[32];
  char       ret_string[64];                      

  if (strncmp(s_s->subtype, "Marker", 1) == 0)  // Read out specified marker
    {
      sprintf(cmd_string, "CALC%d:MARK:Y?%c%c", s_s->num, CR, LF);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));

      if(sscanf(ret_string, "%lf, ", val_out) != 1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read marker!\n", ret_string);
	  return(1);
	}	
    }

    
  else       // Print an error if invalid subtype is entered
    {
      fprintf(stderr, "Wrong type for %s \n", s_s->name);
      return(1);
    } 
  msleep(600);

  return(0);
}


#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
  char       cmd_string[64];
  char       ret_string[64];
  double     ret_val;
  double     set_freq; // in GHz
  
  if (strncmp(s_s->subtype, "Marker", 4) == 0)  // Set the marker value
    {
      if ((s_s->new_set_val < 0.01 ) || (s_s->new_set_val > 50 )) // check valid value
	{
	  fprintf(stderr, "Frequency = %f is bad, must be between 0.01 and 50 GHz \n", s_s->new_set_val);
	  return(1);
	}
   
   
      sprintf(cmd_string, "CALC%d:MARK:X %e%c%c", s_s->num, s_s->new_set_val*1e9, CR, LF);
      fprintf(stderr, "set:\n");
      fprintf(stderr, "%s\n", cmd_string);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      read_tcp(inst_dev, ret_string, sizeof(ret_string)/sizeof(char));

    }

  else       // Print an error if invalid subtype is entered
    {
      fprintf(stderr, "Wrong type for %s \n", s_s->name);
      return(1);
    } 

  return(0);
}

#include "main.h"
