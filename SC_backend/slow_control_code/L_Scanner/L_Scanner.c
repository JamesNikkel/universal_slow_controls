/* Program for Reading out a Keyence IL 1000 laser rangefinder using the DL-EN1 interface*/
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
#define INSTNAME "L_Scanner"

int inst_dev_1;   // Keyence laser rangefinder
int inst_dev_2;   // Arduino motor controller

double X1 = 10;
double X2 = 20;

double x_array[400000];
double y_array[400000];
int    data_length = 0;

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  if ((inst_dev_1 = connect_tcp(i_s)) < 0)
    {
      fprintf(stderr, "Connect failed. \n");
      my_signal = SIGTERM;
      return(1);
    }
  
  
  if ((inst_dev_2 = connect_tcp_raw(i_s->user2, (int)i_s->parm2)) < 0)
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
   close(inst_dev_1);
   close(inst_dev_2);
}


int read_y(double *y_val)
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;
  
  sprintf(cmd_string, "M0%c%c", CR, LF);
	          
  query_tcp(inst_dev_1, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "M0,%d", &return_int) != 1)
    {
      fprintf(stdout, "Bad return string: \"%s\" in read_y!\n", ret_string);
      return(-1);
    }

  *y_val = (double)return_int/1000.0;
  return(0);
}


int read_x(double *x_val)
{
  char       cmd_string[16];
  char       ret_string[16];             
  int        return_int_1;
  int        return_int_2;
  
  sprintf(cmd_string, "%d R 0\n", 2);

  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &return_int_1) != 1)
    {
      //fprintf(stderr, "Bad return string: \"%s\" in read sensor!\n", ret_string);
      return(-1);
    }
  msleep(50);
	    
  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &return_int_2) != 1)
    {
      //fprintf(stderr, "Bad return string: \"%s\" in read sensor!\n", ret_string);
      return(-1);
    }

  if (return_int_1 !=return_int_2)
    return(-1);

  if (return_int_1 < 0)
    return(-1);
  
  *x_val = (double)return_int_1/100.0;
  
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

int read_counter(long *counts)
{
  long       counts_1;
  long       counts_2;
  char       cmd_string[16]; 
  char       ret_string[16];             

  sprintf(cmd_string, "%d C 0\n", 0);

  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if (sscanf(ret_string, "%ld", &counts_1) !=1) 
    {
      //fprintf(stderr, "Bad return string: \"%s\" in read_counter.\n", ret_string);
      return(-1);
    }

  msleep(5);
  
  query_tcp(inst_dev_2, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if (sscanf(ret_string, "%ld", &counts_2) !=1) 
    {
      //fprintf(stderr, "Bad return string: \"%s\" in read_counter.\n", ret_string);
      return(-1);
    }

  //if (counts_1 != counts_2)
  // return(-1);

  if (counts_2 < 0)
    return(-1);

  *counts = counts_2;
  return(0);
}

void reset_counter(void)
{
  char       cmd_string[64];
  sprintf(cmd_string, "%d I 0\n", 2);
  write_tcp(inst_dev_2, cmd_string, strlen(cmd_string));
}

void scan(void)
{
  double x_val, y_val;
  double current_x = -1;
  double target_x;
  long   counts = 0;
  long   prev_counts = -1;
  int i;
  int tries;
  int data_length;


  goto_x(X1);

  while (read_x(&x_val) != 0)
    {
      msleep(100);
    }
  
  reset_counter();
  
  goto_x(X2);
  msleep(10);
  
  i=0;
  tries = 0;
  while (tries<5)
    {
      if (read_counter(&counts) == 0)
	{
	  if (counts == prev_counts)
	    tries++;
	  prev_counts = counts; 
	  read_y(&y_val);
	  current_x = (double)counts * 0.000625 + X1;
	  x_array[i] = current_x;
	  y_array[i] = y_val;
	  i++;
	  //fprintf(stdout, "%lf, %lf \n", current_x, y_val);
	  msleep(50);
	}
    }
}


#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
  char       cmd_string[64];
  char       ret_string[64];             
  int        return_int;

  
  if (strncmp(s_s->subtype, "R", 1) == 0)  // Read out current source position
    {
      sprintf(cmd_string, "M0%c%c", CR, LF);
	    
      //s_s->data_type = DONT_AVERAGE_DATA_OR_INSERT;
      
      query_tcp(inst_dev_1, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
      if(sscanf(ret_string, "M0,%d", &return_int) != 1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read sensor!\n", ret_string);
	  return(1);
	}
      
      *val_out = (double)return_int/1000.0;
      msleep(50);
    }
  return(0);
}

#include "main.h"
