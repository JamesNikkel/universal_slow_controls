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

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "Listen_Daemon"

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
  
  char       ret_string[64];             
  long       ADC_counts;
  int        DIO_val;
    
  if (strncmp(s_s->subtype, "DIO", 3) == 0)  // Do DIO reads
    {
       if (s_s->num < 22 || s_s->num > 53)
	{
	  fprintf(stderr, "Channel must be between 22 and 53 (inclusive), not %d.\n", s_s->num);
	  return(1);
	}

      sprintf(cmd_string, "RD %d\n", s_s->num);

      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(200);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
     
      if (sscanf(ret_string, "%d", &DIO_val) !=1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" \n", ret_string);
	  return(1);
	}
      if (DIO_val > 0)
	*val_out = 1;
      else
	*val_out = 0; 
    }
  else if  (strncmp(s_s->subtype, "AN", 2) == 0)  // Do analogue reads
    {
      if (s_s->num < 0 || s_s->num > 15)
	{
	  fprintf(stderr, "Channel must be between 0 and 15 (inclusive), not %d.\n", s_s->num);
	  return(1);
	}

      sprintf(cmd_string, "RA %d\n", s_s->num);

      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(200);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

      if (sscanf(ret_string, "%ld", &ADC_counts) !=1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" \n", ret_string);
	  return(1);
	}
    
      if (ADC_counts < 0)
	{
	  fprintf(stderr, "Bad return value: \"%s\" \n", ret_string);
	  return(1);
	}

      *val_out = ( s_s->parm1 + (double)ADC_counts ) * s_s->parm2;  // standard conversion where parm1 is offset in ADC counts and parm2 is linear term
    }
  else       // Print an error if invalid subtype is entered
    {
      fprintf(stderr, "Wrong type for %s \n", s_s->name);
      return(1);
    } 

  sleep(1);

  return(0);
}

#include "main.h"