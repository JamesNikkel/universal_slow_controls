/* Program for reading a custom made Arduino POE ADC  */
/* and putting said readings in to a mysql database. */
/* James Nikkel */
/* james.nikkel@rhul.ac.uk */
/* Copyright 2013 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "Arduino_ADC"

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
  long       ADC_counts;

  sleep(1);
  
  if (s_s->num < 0 || s_s->num > 15)
    {
      fprintf(stderr, "Channel must be between 0 and 5 (inclusive), not %d.\n", s_s->num);
      return(1);
    }

  sprintf(cmd_string, "%d\n", s_s->num);

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

  return(0);
}

#include "main.h"
