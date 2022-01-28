/* Program for reading the Agilent VNA */
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
#define BUSY_WAIT_TIME 30

int inst_dev;

struct VNA_scan_setup_struct {
  char   name[18];
  int    time;
  double value;
  double rate;
  double start_freq;
  double stop_freq;
  int    num_points;
  double source_power;
  int    avg_state;
  char   avg_mode[8];
  int    avg_factor;
  char   sweep_type[8];
  char   sweep_mode[8];
  int    point_sweep;
  int    gate_state;
  double gate_start;
  double gate_stop;
  char   gate_mode[8];
  char   cal_set_name[128];
  double cntrl_start;
  double cntrl_stop;
  double cntrl_step;
  char   notes[1024];
};

struct VNA_scan_setup_struct *VNA_ss;

char   VNA_data[200000];
int    current_setup_num;
double current_cntrl_val; 
int    scan_running;
char   cntrl_name[18];
char   cntrl_set_name[18];
char   cntrl_status_name[18];
char   run_name[18];
char   data_name[18];
char   setup_name[18];

int populate_setup_struct(void)
{
  char       cmd_string[32];
  char       ret_string[128];              
  double     dbl_val;
  int        int_val;
  char       string_val[128];

  sprintf(cmd_string, "*CLS\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  
  msleep(100);
  
  VNA_ss->time = time(NULL); 

  sprintf(cmd_string, "SENS:FREQ:STAR?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

  if(sscanf(ret_string, "%lf", &dbl_val) != 1) {
      fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
      return(1);}
  VNA_ss->start_freq = dbl_val;
  msleep(50);

  
  sprintf(cmd_string, "SENS:FREQ:STOP?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%lf", &dbl_val) != 1) {
      fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
      return(1);}
  VNA_ss->stop_freq = dbl_val;
  msleep(50);
  
  sprintf(cmd_string, "SENS:SWE:POIN?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
      fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
      return(1);}
  VNA_ss->num_points = int_val;
  msleep(50);
  
  sprintf(cmd_string, "SENS:SWE:POIN?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
      fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
      return(1);}
  VNA_ss->num_points = int_val;
  msleep(50);
  
  sprintf(cmd_string, "SOUR:POW?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%lf", &dbl_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->source_power = dbl_val;
  msleep(50);

  sprintf(cmd_string, "SENS:AVER?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->avg_state = int_val;
  msleep(50);

  sprintf(cmd_string, "SENS:AVER:MODE?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%s", string_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  snprintf(VNA_ss->avg_mode, sizeof(VNA_ss->avg_mode), string_val);
  msleep(50);

  sprintf(cmd_string, "SENS:AVER:COUN?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->avg_factor = int_val;
  msleep(50);

  sprintf(cmd_string, "SENS:SWE:TYPE?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%s", string_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  snprintf(VNA_ss->sweep_type, sizeof(VNA_ss->sweep_type), string_val);
  msleep(50);

  sprintf(cmd_string, "SENS:SWE:GEN?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%s", string_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  snprintf(VNA_ss->sweep_mode, sizeof(VNA_ss->sweep_mode), string_val);
  msleep(50);

  sprintf(cmd_string, "SENS:SWE:GEN:POIN?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->point_sweep = int_val;
  msleep(50);

  sprintf(cmd_string, "CALC:FILT:TIME:STAT?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%d", &int_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->gate_state = int_val;
  msleep(50);

  sprintf(cmd_string, "CALC:FILT:TIME:STAR?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%lf", &dbl_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->gate_start = dbl_val;
  msleep(50);

  sprintf(cmd_string, "CALC:FILT:TIME:STOP?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%lf", &dbl_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  VNA_ss->gate_stop = dbl_val;
  msleep(50);

  sprintf(cmd_string, "CALC:FILT:TIME?\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%s", string_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  snprintf(VNA_ss->gate_mode, sizeof(VNA_ss->gate_mode), string_val);
  msleep(50);

  /*
  sprintf(cmd_string, "SENS:COR:CSET:ACT? name\n");
  query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
  if(sscanf(ret_string, "%s", string_val) != 1) {
    fprintf(stderr, "Bad return string: \"%s\" in %s", ret_string, cmd_string);
    return(1);}
  snprintf(VNA_ss->cal_set_name, sizeof(VNA_ss->cal_set_name), string_val);
  msleep(50);
  */

  return(0);
}


int insert_mysql_VNA_setup_data(void)
{
  char query_strng[2048];  
  
  sprintf(query_strng, 
	  "INSERT INTO `sc_sens_%s` ( `time`, `value`, `rate`, `start_freq`, `stop_freq`, `num_points`, `source_power`, `avg_state`, `avg_mode`, `avg_factor`, `sweep_type`, `sweep_mode`, `point_sweep`, `gate_state`, `gate_start`, `gate_stop`, `gate_mode`, `cal_set_name`, `cntrl_start`, `cntrl_stop`, `cntrl_step`, `notes`) VALUES ( %lu, %f, %f, %f, %f, %d, %f, %d, \"%s\", %d, \"%s\", \"%s\", %d, %d, %f, %f, \"%s\", \"%s\", %f, %f, %f, \"%s\" )",
	  setup_name, 
	  VNA_ss->time, VNA_ss->value, VNA_ss->rate,
	  VNA_ss->start_freq, VNA_ss->stop_freq, VNA_ss->num_points, VNA_ss->source_power,
	  VNA_ss->avg_state, VNA_ss->avg_mode, VNA_ss->avg_factor, 
	  VNA_ss->sweep_type, VNA_ss->sweep_mode, VNA_ss->point_sweep, 
	  VNA_ss->gate_state, VNA_ss->gate_start, VNA_ss->gate_stop, VNA_ss->gate_mode, 
	  VNA_ss->cal_set_name, 
	  VNA_ss->cntrl_start, VNA_ss->cntrl_stop, VNA_ss->cntrl_step, 
	  VNA_ss->notes);

  return(write_to_mysql(query_strng)); 
}

int get_setup_num(void)
{
  char  query_strng[512]; 
  char  res_strng[512];
  int num_rows = 0;
  int num_cols = 0;
  int int_out;

  sprintf(query_strng, "SELECT setup_num FROM `sc_sens_%s` ORDER BY `time` DESC LIMIT 1", setup_name);
  if(process_statement(query_strng, res_strng, &num_rows, &num_cols))
      return(-1);
  
  if (sscanf(res_strng, "%lu", &int_out) == 1)
      return(int_out);
  return(-1);
}

int start_run(void)
{  
  if (scan_running == 0)
    {
      populate_setup_struct();

      if ((VNA_ss->cntrl_stop < VNA_ss->cntrl_start) && (VNA_ss->cntrl_step > 0))
	{
	  fprintf(stderr, "Scan parameters are nonsensical!\n");
	  return(1);
	}
      current_cntrl_val = VNA_ss->cntrl_start;

      if (insert_mysql_VNA_setup_data())
      	return(1);
      
      current_setup_num = get_setup_num();
       
      insert_mysql_sensor_data(cntrl_set_name, time(NULL), current_cntrl_val, 0.0);
      
      scan_running = 1;
    }
  return(0);
}


int read_VNA_data(void)
{
  char cmd_string[32];
  char query_strng[210000];  
  int  ret_val;
  
  sprintf(cmd_string, "*CLS\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  
  msleep(100);

  sprintf(cmd_string, "CALC:PAR:SEL \"CH1_S11_1\"\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));

  msleep(100);

  sprintf(cmd_string, "CALC:DATA:SNP:PORTS? \"1, 2\"\n");
  //ret_val = query_tcp(inst_dev, cmd_string, strlen(cmd_string), VNA_data, sizeof(VNA_data)/sizeof(char));
  ret_val =  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
  ret_val += read_long_tcp(inst_dev, VNA_data, sizeof(VNA_data)/sizeof(char), 10);
  
  sprintf(query_strng, "INSERT INTO `sc_sens_%s` ( `time`, `value`, `rate`, `cntrl_val`, `setup_num`, `data`)  VALUES ( %lu, %f, %f, %f, %d, \"%s\") ", 
	  data_name, time(NULL), 1.0*current_setup_num, 0.0,
	  current_cntrl_val,
	  current_setup_num,
	  VNA_data );

  ret_val += write_to_mysql(query_strng);
  
  return (ret_val);
}


int continue_run(void)
{  
  if (current_cntrl_val <= VNA_ss->cntrl_stop)
    {
      if (read_VNA_data())
	{
	  fprintf(stderr, "Read VNA Data failed. \n");
	  return(1);
	}
      
      current_cntrl_val += VNA_ss->cntrl_step;
      insert_mysql_sensor_data(cntrl_set_name, time(NULL), current_cntrl_val, 0.0);
      
      return(0);
    }
  else
    {
      scan_running = 0;
      insert_mysql_sensor_data(run_name, time(NULL), 0.0, 0.0);
      return(0);
    }
}

int is_cntrl_busy(void)
{
  /// check to see if the control for the scan is busy or not.
  //  returns 0 if the control is stable, 1 if it's still busy (moving, unstable, etc.)
  time_t t_out;
  double v_out;
  double r_out;
  time_t current_time; 
 
  current_time = time(NULL);
  read_mysql_sensor_data (cntrl_status_name, &t_out, &v_out, &r_out);
  if (v_out > 0.5)
    return(1);
  
  if (current_time - t_out  > BUSY_WAIT_TIME)
    return(1);

  return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  struct sensor_struct *s_s;
  char       cmd_string[32];
  char       ret_string[64];  
  time_t     t_out;
  double     v_out;
  double     r_out;
  int        i;

  VNA_ss = malloc(sizeof(struct VNA_scan_setup_struct));
  if (VNA_ss == NULL)		
    {
      fprintf(stderr, "Malloc failed\n");
      return(1);
    }
  
  scan_running = 0;

  if ((inst_dev = connect_tcp(i_s)) < 0)
    {
      fprintf(stderr, "Connect failed. \n");
      my_signal = SIGTERM;
      return(1);
    }
  
  msleep(2000);
  sprintf(cmd_string, "*CLS\n");
  write_tcp(inst_dev, cmd_string, strlen(cmd_string));
        
  msleep(100);
  		  
  for(i=0; i < i_s->num_active_sensors; i++)
    {
      s_s = &s_s_a[i];
      if (!(is_null(s_s->name)))
	{
	  s_s->data_type = DONT_AVERAGE_DATA_OR_INSERT;
	  if (strncmp(s_s->subtype, "Data", 4) == 0)
	    {
	      snprintf(data_name, 16, "%s", s_s->name);
	    }
	  if (strncmp(s_s->subtype, "Run", 3) == 0)  
	    {
	      snprintf(cntrl_name, 16, "%s", s_s->user1);
	      snprintf(cntrl_set_name, 16, "%s", s_s->user2);
	      snprintf(cntrl_status_name, 16, "%s", s_s->user3);
	      snprintf(run_name, 16, "%s", s_s->name);
	    }
	  if (strncmp(s_s->subtype, "Setup", 5) == 0)  
	    {
	      snprintf(setup_name, 16, "%s", s_s->name);
	    }
	  if (strncmp(s_s->subtype, "Start", 5) == 0)  
	    {
	      read_mysql_sensor_data (s_s->name, &t_out, &v_out, &r_out);
	      VNA_ss->cntrl_start = v_out;
	    }
	  if (strncmp(s_s->subtype, "Stop", 4) == 0)  
	    {
	      read_mysql_sensor_data (s_s->name, &t_out, &v_out, &r_out);
	      VNA_ss->cntrl_stop = v_out;
	    }
	  if (strncmp(s_s->subtype, "Step", 4) == 0)  
	    {
	      read_mysql_sensor_data (s_s->name, &t_out, &v_out, &r_out);
	      VNA_ss->cntrl_step = v_out;
	    }
	    
	}
    }
  
  msleep(100);

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

  if (strncmp(s_s->subtype, "Marker", 4) == 0)  // Read out specified marker
    {
      if (is_cntrl_busy() == 1)
	{
	  msleep(1000);
	//  return(0);
	}
      sprintf(cmd_string, "*CLS\n");
      write_tcp(inst_dev, cmd_string, strlen(cmd_string));

      msleep(100);

      sprintf(cmd_string, "CALC:PAR:SEL %s\n", s_s->user1);
      write_tcp(inst_dev, cmd_string, strlen(cmd_string));

      msleep(100);
 
      sprintf(cmd_string, "CALC:MARK%d:Y?\n", s_s->num);
      query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));

      if(sscanf(ret_string, "%lf, ", val_out) != 1)
	{
	  fprintf(stderr, "Bad return string: \"%s\" in read marker!\n", ret_string);
	  return(1);
	}
      add_val_sensor_struct(s_s, time(NULL), *val_out);
      s_s->rate = 0.0;
      write_temporary_sensor_data(s_s);
      return(insert_mysql_sensor_data(s_s->name, s_s->times[s_s->index], s_s->vals[s_s->index], s_s->rate));
    }
  
  else if (strncmp(s_s->subtype, "Data", 4) == 0) 
    {
      if (scan_running == 0)
	{
	  msleep(1000);
	  return(0);
	}
      
      if (is_cntrl_busy() == 1)
	{
	  msleep(1000);
	  return(0);
	}
      
      continue_run();
      msleep(1000);
      return(0);
    }
  
  msleep(1000);

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
   
      sprintf(cmd_string, "CALC:PAR:SEL %s\n", s_s->user1);
      write_tcp(inst_dev, cmd_string, strlen(cmd_string));

      msleep(100);

      sprintf(cmd_string, "CALC:MARK%d:X %e\n", s_s->num, s_s->new_set_val*1e9);
      write_tcp(inst_dev, cmd_string, strlen(cmd_string));

    }
  else if (strncmp(s_s->subtype, "Start", 5) == 0)  
    {
      VNA_ss->cntrl_start = s_s->new_set_val;
    }
  else if (strncmp(s_s->subtype, "Stop", 4) == 0)  
    {
      VNA_ss->cntrl_stop = s_s->new_set_val;
    }
  else if (strncmp(s_s->subtype, "Step", 4) == 0)  
    {
      VNA_ss->cntrl_step = s_s->new_set_val;
    }

  else if (strncmp(s_s->subtype, "Run", 3) == 0)  
    {
      if (s_s->new_set_val > 0.5 )
	{
	  if(start_run())
	    {
	      fprintf(stderr, "Start run failed ( %s ) \n", s_s->name);
	      insert_mysql_sensor_data(run_name, time(NULL), 0.0, 0.0);
	      scan_running = 0;
	    }
	}
      else
	{
	  scan_running = 0;
	}
    }
  
  else if (strncmp(s_s->subtype, "Single", 6) == 0)  
    {
      if (populate_setup_struct())
	{
	  fprintf(stderr, "populate_setup_struct() failed.\n");
	  return(1);
	}
      
      if (insert_mysql_VNA_setup_data())
	{
	  fprintf(stderr, "insert_mysql_VNA_setup_data() failed.\n");
	  return(1);
	}
      current_setup_num = get_setup_num();
     
      if (read_VNA_data())
	{
	  fprintf(stderr, "read_VNA_data() failed.\n");
	  return(1);
	}
    }

  return(0);
}

#include "main.h"
