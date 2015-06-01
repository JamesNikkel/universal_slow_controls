/* Program for reading a LakeShore335 temp. controller */
/* and putting said readings in to a mysql database. */
/* defined below. */
/* Code by James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2006, 2007, 2015 */
/* James public licence. */


#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "ethernet.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "LS331"

int inst_dev;

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  char       cmd_string[64];

  if ((inst_dev = connect_tcp(i_s)) < 0)
    {
      fprintf(stderr, "Connect failed. \n");
      my_signal = SIGTERM;
      return(1);
    }

  sprintf(cmd_string, "++mode 1\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  msleep(200);

  sprintf(cmd_string, "++auto 1\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  msleep(200);

  sprintf(cmd_string, "++addr 12\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  msleep(200);

  sprintf(cmd_string, "*CLS\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  msleep(200);

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
  double     P_range;
  double     P_percent;


  if (strncmp(s_s->subtype, "Temp", 1) == 0)  // Read out value for temperature from A,B,C or D.
    {
      sprintf(cmd_string, "INPUT? %c\n", int_to_Letter(s_s->num));
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      msleep(200);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      
      if(sscanf(ret_string, "%lf", val_out) != 1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read temperature!\n", ret_string);
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

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
    char   cmd_string[32];
    char   val[32];                      
    double P_percent;

    if (strncmp(s_s->subtype, "temp", 3) == 0) // temperatures
    {
	if ((s_s->num < 1) || (s_s->num > 2))
	{
	    fprintf(stderr, "Channel number for %s must be 1 or 2 \n", s_s->name);
	    return(1);
	}
	
	sprintf(cmd_string, "KRDG? %c\r\n", int_to_Letter(s_s->num));
	ibwrt(inst_dev, cmd_string, strlen(cmd_string));
	msleep(100);  
	ibrd(inst_dev, &val, 30);
	sscanf(val, "%lf", val_out);	
    }
    else if (strncmp(s_s->subtype, "power", 3) == 0)   // heaters 
    {
	sprintf(cmd_string, "HTR?\r\n");
	ibwrt(inst_dev, cmd_string, strlen(cmd_string));
	msleep(100);  
	ibrd(inst_dev, &val, 30);
	sscanf(val, "%f", &P_percent);
	*val_out = P_percent;
    }
    else
    {
	fprintf(stderr, "Wrong subtype for %s \n", s_s->name);
	return(1);
    }
    return(0);
}


#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
    char cmd_string[64];
    
    if (s_s->new_set_val < 0 )
    {
	fprintf(stderr, "Setpoint for %s must be positive \n", s_s->name);
	return(1);
    }
    if (strncmp(s_s->subtype, "setpoint", 3) == 0)     // closed loop setpoint
    {
	if ((s_s->num < 1) || (s_s->num > 2))
	{
	    fprintf(stderr, "Loop number for %s must be 1 or 2 \n", s_s->name);
	    return(1);
	}
	sprintf(cmd_string, "SETP %d,%f\n", s_s->num, s_s->new_set_val);
	ibwrt(inst_dev, cmd_string, strlen(cmd_string));
	msleep(500);
    }
    else
    {
	fprintf(stderr, "Wrong subtype for %s \n", s_s->name);
	return(1);
    }
    return(0);  
}

#include "main.h"
