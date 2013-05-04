/* Program for reading out the SRS residual gas analyzer using the rs232 interface */
/* and putting said readings into a mysql database. */
/* Operational parameters are taken from the database */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2006, 2009 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "serial.h"
#include "ethernet.h"

#define INSTNAME "SRS_RGA"

int   comm_type = -1;
int   inst_dev;


///////////////////////////////////////////////////////////////////////////////////////////////

int conv_to_current(char *val, size_t n)
{
    union {int iVal; unsigned char bytes[4]; } value;     //  Again with the union magic
    if (n<4)
    {
	fprintf(stderr, "conv_to_current val too small\n");
	return(-1);
    }
    value.bytes[0] = val[0];
    value.bytes[1] = val[1];
    value.bytes[2] = val[2];
    value.bytes[3] = val[3]; 

    return(value.iVal);
} 

int read_ion_val(char *val)
{
    //  returns the current in units of 10^-16 A
    int num_bytes = 4;
    int i = 0;
    int tries = 0;
    int rdstatus;
    do
    {
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    rdstatus = read_serial(inst_dev, &val[i], 1);
	else                               // do tcp write/read
	    rdstatus = read_tcp(inst_dev, &val[i], 1);
	
	if  (rdstatus == 0)
	    i++;
	else
	{
	    tries++;
	    if (tries > 100)
		return(1);
	    wait(100);
	}
    } while (i < num_bytes);  
    return(0);
}

int wait_for_status_byte(struct inst_struct *i_s, struct sensor_struct *s_s)
{
    char     val[16];
    int      rdstatus; 
    int      status_byte;
    int      sleep_time = 200;        // (ms)
    time_t   start_time = time(NULL);
    time_t   cur_time = time(NULL);
    time_t   max_wait_time = 20*60;   // (s) Wait for 20 minutes

    memset(val, 0, sizeof(val));

    do
    {
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    rdstatus = read_serial(inst_dev, val, sizeof(val)-1);
	else                               // do tcp write/read
	    rdstatus = read_tcp(inst_dev, val, sizeof(val)-1);
	
	if (rdstatus == 0)
	{
	    sscanf(val, "%d", &status_byte);
	    return(status_byte);
	}
	else
	{
	    if (time(NULL) - cur_time > INST_TABLE_UPDATE_PERIOD)
	    {
		mysql_inst_run_status(i_s);
		cur_time = time(NULL);
	    }
	    msleep(sleep_time);
	    cur_time += sleep_time;
	}
    } while (time(NULL) - start_time < max_wait_time);
    fprintf(stderr, "wait_for_status_byte had to wait too long.  \n");
    return(1);  
}

///////////////////////////////////////////////////////////////////////////////////////////////

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)  
{
    char       cmd_string[1024]; 
    char       ret_string[1024];
    int        status_byte;
    int        status = 0;
    char       current[4];
    
    struct  termios    tbuf;  // serial line settings 
   
    if (strncmp(i_s->dev_type, "serial", 6) == 0) 
    {
	comm_type = TYPE_SERIAL;
	if (( inst_dev = open(i_s->dev_address, (O_RDWR | O_NDELAY), 0)) < 0 ) 
	{
	    fprintf(stderr, "Unable to open tty port specified:\n");
	    my_signal = SIGTERM;
	    return(1);
	}

	// set up the serial line parameters :
    
	msleep(200);

	tbuf.c_cflag = CS8|CREAD|B38400|CLOCAL|CRTSCTS;
	tbuf.c_iflag = 0;
	tbuf.c_oflag = 0;
	tbuf.c_lflag = 0;
	tbuf.c_cc[VMIN] = 0;
	tbuf.c_cc[VTIME]= 0; 
	//tcflush(inst_dev, TCIFLUSH);
    
	//cfsetispeed(&tbuf, B38400);
	//cfsetospeed(&tbuf, B38400);
    
	if (tcsetattr(inst_dev, TCSANOW, &tbuf) < 0)
	{
	    fprintf(stderr, "Unable to set device '%s' parameters\n", i_s->dev_address);
	    my_signal = SIGTERM;
	    return(1);
	}
    
	/*

	  struct serial_struct serial_info;
	  serial_info.reserved_char[0] = 0;

	  serial_info.flags = ASYNC_SPD_CUST | ASYNC_LOW_LATENCY;
	  serial_info.custom_divisor = 833;  // clock on FTDI chip / 833 ~ 28.8KB
    
	  if ( ioctl(inst_dev, TIOCSSERIAL, &serial_info ) < 0) {
	  perror("config_serial_port: ioctl TIOCSSERIAL");
	  return(-1);
	  }
    

	  msleep(200);
	*/
    }
    else if (strncmp(i_s->dev_type, "eth", 3) == 0) 
    {
	comm_type = TYPE_ETH;
	if ((inst_dev = connect_tcp(i_s)) < 0)
	{
	    fprintf(stderr, "Connect failed. \n");
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

    sprintf(cmd_string, "%c%c", CR, CR);  // Clear buffer
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
   
    sprintf(cmd_string, "IN0%c", CR);   //  Send initilize command
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
  
    sprintf(cmd_string, "ID?%c", CR);    //  Check device ID
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    fprintf(stdout, "ID?--- %d: %s \n", status, ret_string);
    msleep(500);
   
    sprintf(cmd_string, "ER?%c", CR);   // Query status byte
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    sscanf(ret_string, "%d", &status_byte);
    if (status_byte > 0)
	fprintf(stderr, "Status Byte:  %d \n", status_byte);
    msleep(500);

    sprintf(cmd_string, "HV0%c", CR);   // Turn off electron multiplier
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
 
    sprintf(cmd_string, "EE70%c", CR);  //  Set electron energy to 70 eV
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);

    sprintf(cmd_string, "IE1%c", CR);   // Set ion energy to high
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    
    sprintf(cmd_string, "VFL100%c", CR);  // Set focus voltage to -100V
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    
    sprintf(cmd_string, "FL1.0%c", CR); // Set filament current to 1 mA and turn on
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
   
    sprintf(cmd_string, "NF%d%c", (int)i_s->parm1, CR);          //  Set noise floor value from instrument parm1
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(1000);
   
    sprintf(cmd_string, "TP1%c", CR);   //  Request that the total pressure be measured.  
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
   
    sprintf(cmd_string, "TP?%c", CR);   //  Read the total pressure value
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);

    sprintf(cmd_string, "MR2%c", CR);   //  Turn on RF and DC by requesting a single AMU = 2 (hydrogen) measurement 
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);

    return(0);
}

#define _def_clean_up_inst
void clean_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)   
{
    char       cmd_string[1024];
    char       ret_string[1024];                      
    int        status = 0;
    
    
    sprintf(cmd_string, "MR0%c", CR);   //  Turn off RF and DC to quadrapole
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    
    sprintf(cmd_string, "HV0%c", CR);   // Turn off electron multiplier
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
 
    sprintf(cmd_string, "FL0%c", CR);    //  Turn off filament current
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    close(inst_dev);
}

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
    // Reads out the current value of the device
    // at given sensor.
    
    char       cmd_string[1024];
    char       ret_string[1024];
    
    int        status;
    double     SP_Parm = 1;     // mA/Torr
    double     ST_Parm = 1;     // mA/Tort
    double     CDEM_gain = 1;   // gain of the electron multiplier
    int        HV;
    int        x0, x1, dxN, num_points;
    int        i;
    char       y_array[20000];
    char       y_temp[1024];
    int        y[10000];
    memset(y, 0, sizeof(y));

    sprintf(cmd_string, "%c%c", CR, CR);  // Clear buffer
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    
    sprintf(cmd_string, "SP?%c", CR);   // get partial pressure conv. factor
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    sscanf(ret_string, "%f", &SP_Parm);
    
    sprintf(cmd_string, "HV?%c", CR);         //  Read CEM voltage
    if (comm_type == TYPE_SERIAL)	// do serial write/read
	status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    else                               // do tcp write/read
	status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
    msleep(500);
    sscanf(ret_string, "%d", &HV);
    
    if (HV >=10)
    {
	sprintf(cmd_string, "MG?%c", CR);                             // get electron multiplier gain
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(200);
	
	sscanf(ret_string, "%f", &CDEM_gain);	
    }
    

    if (strncmp(s_s->subtype, "single", 4) == 0)  // Single mass measurement
    {
	sprintf(cmd_string, "MR%d%c", s_s->num, CR);
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(200);
	*val_out = 1e-4/SP_Parm/CDEM_gain*conv_to_current(ret_string, 4);
    }
    
    else if (strncmp(s_s->subtype, "analog", 4) == 0)  // Analogue scan mode
    {
	s_s->data_type = ARRAY_DATA;

	sprintf(cmd_string, "ST?%c", CR);            // get total pressure conv. factor
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(200);
	sscanf(ret_string, "%f", &ST_Parm);

	sprintf(cmd_string, "MI%d%c", (int)s_s->parm1, CR);        // Set start mass
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);

	sprintf(cmd_string, "MI?%c", CR);        // Check set of start mass
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);
	
	sscanf(ret_string, "%d", &x0);
	if (x0 != (int)s_s->parm1)
	{
	    fprintf(stderr, "Could not set MI on %s (1 to MF) (%d != %d) \n ", s_s->name, x0, (int)s_s->parm1);
	    return(1);
	} 

	sprintf(cmd_string, "MF%d%c", (int)s_s->parm2, CR);         // Set stop mass
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);
	
	sprintf(cmd_string, "MF?%c", CR);         // Check set of stop mass
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);

	sscanf(ret_string, "%d", &x1);
	if (x1 != (int)s_s->parm2)
	{
	    fprintf(stderr, "Could not set MF on %s (MI to 200) (%d != %d) \n ", s_s->name, x1, (int)s_s->parm2);
	    return(1);
	} 

	sprintf(cmd_string, "SA%d%c", (int)s_s->parm3, CR);        // Set number of points per AMU
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);
	
	sprintf(cmd_string, "SA?%c", CR);        // Check number of points per AMU
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);
    
	sscanf(ret_string, "%d", &dxN);
	if (dxN != (int)s_s->parm3)
	{
	    fprintf(stderr, "Could not set SA on %s (10-25) (%d != %d) \n ", s_s->name, dxN, (int)s_s->parm3);
	    return(1);
	} 

	sprintf(cmd_string, "AP?%c", CR);  // Count size of analog run
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, strlen(cmd_string), ret_string, sizeof(ret_string)/sizeof(char));
	msleep(100);

	sscanf(ret_string, "%d", &num_points);
	

	////////////////do trigger!
	//fprintf(stdout, "Starting analog run, %d to %d, (N = %d) \n ", x0, x1, num_points);
	sprintf(cmd_string, "SC1%c", CR);
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = write_serial(inst_dev, cmd_string, strlen(cmd_string));
	else                               // do tcp write/read
	    status = write_tcp(inst_dev, cmd_string, strlen(cmd_string));
	msleep(100);
	
	memset(y_array, 0, sizeof(y_array));
	sprintf(y_array, "");

	//  Read out points:
	int num_read = 0;
	for (i = 0; i < num_points-1; i++)
	{
	    memset(ret_string, 0, sizeof(ret_string));
	    if (read_ion_val(ret_string) == 0)
	    {
		y[i] = conv_to_current(ret_string, sizeof(ret_string));
		sprintf(y_temp, "%d,", y[i]);	    
		strcat(y_array, y_temp);
		num_read++;
	    }
	}
	memset(ret_string, 0, sizeof(ret_string));
	if (read_ion_val(ret_string) == 0)
	{
	    y[i] = conv_to_current(ret_string, sizeof(ret_string));
	    sprintf(y_temp, "%d", y[i]);
	    strcat(y_array, y_temp);
	    num_read++;
	}
	if (num_read < num_points)
	{
	    fprintf(stderr, "Only read out %d points.\n ", num_read);
	    return(1);
	}

	//  Now read total pressure value:
	memset(ret_string, 0, sizeof(ret_string));
	if (read_ion_val(ret_string))
	    return(1);
	
	*val_out = 1e-4/ST_Parm/CDEM_gain * conv_to_current(ret_string, sizeof(ret_string));    // convert to pTorr
	insert_mysql_sensor_array_data(s_s->name, time(NULL), *val_out, s_s->rate, (double)x0, (double)x1, dxN, 1e-4/SP_Parm/CDEM_gain, y_array);
    }

    else if (strncmp(s_s->subtype, "histogram", 4) == 0)  // Histogram scan mode
    {
	// do stuff
	;
    }
    else 
    {
	fprintf(stderr, "No such measurement type: %s \n", s_s->name);
	return(1);
    }

    return(0);    
}

#define _def_set_sensor
int set_sensor(struct inst_struct *i_s, struct sensor_struct *s_s)
{
    char       cmd_string[1024];
    char       ret_string[1024];
    int        status;
    int        HV;
   
    if (strncmp(s_s->subtype, "CDEM", 4) == 0)  // Turn on electron multiplier and set to given value
    {
	if (s_s->new_set_val < 10)
	    sprintf(cmd_string, "HV0%c", CR);       //  Turn off electron multiplier
	else if (s_s->new_set_val > 2490)
	{
	    fprintf(stderr, "Electron multiplier voltage must be 0, or between 10 and 2490\n");
	    return(1);
	}
	else
	    sprintf(cmd_string, "HV%d%c", (int)s_s->new_set_val, CR);         // Set electron multiplier voltage in Volts
     
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
	msleep(1000);

	sprintf(cmd_string, "HV?%c", CR);         
	if (comm_type == TYPE_SERIAL)	// do serial write/read
	    status = query_serial(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
	else                               // do tcp write/read
	    status = query_tcp(inst_dev, cmd_string, 4, ret_string, sizeof(ret_string)/sizeof(char));
	
	sscanf(ret_string, "%d", &HV);
	if (abs(HV - (int)s_s->new_set_val) > s_s->new_set_val*0.05)     // make sure it is within 5%
	    return(1);
	return(0);
    }
    else  if (strncmp(s_s->subtype, "CALIBRATE", 4) == 0)  // Run calibration routine
    {
	if (s_s->new_set_val == 1)
	{
	    mysql_inst_run_status(i_s);
	    sprintf(cmd_string, "CL%c", CR);         // Calibrate IV response
	    if (comm_type == TYPE_SERIAL)	// do serial write/read
		status = write_serial(inst_dev, cmd_string,  strlen(cmd_string));
	    else                               // do tcp write/read
		status = write_tcp(inst_dev, cmd_string,  strlen(cmd_string));
	    msleep(1000);
	    if ((status = wait_for_status_byte(i_s, s_s)) != 0)
	    {
		fprintf(stderr, "CL status bit: %d \n", status);
		return(1);
	    }
	    
	    mysql_inst_run_status(i_s);
	    msleep(1000);
	    sprintf(cmd_string, "CA%c", CR);         // Calibrate all
	    if (comm_type == TYPE_SERIAL)	// do serial write/read
		status = write_serial(inst_dev, cmd_string, strlen(cmd_string));
	    else                               // do tcp write/read
		status = write_tcp(inst_dev, cmd_string, strlen(cmd_string));
	    msleep(1000);
	    if ((status = wait_for_status_byte(i_s, s_s)) != 0)
	    {
                fprintf(stderr, "CA status bit: %d \n", status);
                return(1);
	    }
	}
	else if (s_s->new_set_val == 0)
	    return(0);
	
	insert_mysql_sensor_data(s_s->name, time(NULL), 0, 0);
	s_s->last_set_time = time(NULL);
	return(0);
    }
    else 
    {
	fprintf(stderr, "No such control type: %s \n", s_s->name);
	return(1);
    }
    
    return(0);
}


#include "main.h"
