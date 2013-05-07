/* Program for reading an MKS PDR2000 pressure controller using the rs232 interface */
/* and putting said readings into a mysql database. */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2006, 2009 */
/* James public licence. */

#include <stdlib.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "serial.h"

#define INSTNAME "PDR2000"

int inst_dev;

#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s)  
{
    struct  termios    tbuf;  // serial line settings 
    char               val[64];       
    ssize_t            wstatus = 0;
    ssize_t            rdstatus = 0;

    if (( inst_dev = open(i_s->dev_address, (O_RDWR | O_NDELAY), 0)) < 0 ) 
    {
        fprintf(stderr, "Unable to open tty port specified:\n");
	my_signal = SIGTERM;
	return(1);
    }
    
    // set up the serial line parameters : 8,1,N 
    tbuf.c_cflag = CS8|CREAD|B9600|CLOCAL;
    tbuf.c_iflag = IGNBRK;
    tbuf.c_oflag = 0;
    tbuf.c_lflag = 0;
    tbuf.c_cc[VMIN] = 0; // character-by-character input
    tbuf.c_cc[VTIME]= 0; // no delay waiting for characters
    if (tcsetattr(inst_dev, TCSANOW, &tbuf) < 0) {
        fprintf(stdout, "Unable to set device '%s' parameters\n", i_s->dev_address);
        exit(1);
    }
    
    wstatus = write(inst_dev, "p", sizeof(char));      // auto-sets the baud rate
    sleep(1);
    rdstatus = read(inst_dev, &val, 32);
   
    tcflush(inst_dev, TCIFLUSH); 
   
    wstatus = write(inst_dev, "p", sizeof(char));      // auto-sets the baud rate
    sleep(1);
    rdstatus = read(inst_dev, &val, 32);
   
    return(0);
}

#define _def_clean_up_inst
void clean_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s)   
{
    close(inst_dev);
}

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{   
    // Reads out the current value of the device
    // at given sensor.  0 is the first, 1, second, ...

    char       val[64];                      
    double     val_out1 = 0;   
    double     val_out2 = 0;   
    size_t     wstatus = 0;
    ssize_t    rdstatus = 0;
    int        sstatus = 0;

    wstatus = write(inst_dev, "p", sizeof(char));
    msleep(100);
    rdstatus = read(inst_dev, &val, 32);
    //fprintf(stdout, "Ret from dev: \n%s\n", val);
    msleep(800);
    sstatus = sscanf(val, "%le %le", &val_out1, &val_out2);
    if (sstatus < 2)
    {
	sstatus = sscanf(val, "Low %le", &val_out2);
	if (sstatus == 1)
	    val_out1 = 0;
	else
	{
	    sstatus = sscanf(val, "%le Low", &val_out1);
	    if (sstatus == 1)
		val_out2 = 0;
	    else
	    {
		fprintf(stderr, "Could not read instrument.");
		return(1);
	    }
	}
    }

    if (s_s->num == 1)
	*val_out = val_out1;	
    else  if (s_s->num == 2)
	*val_out = val_out2;		
    else
    {
	fprintf(stderr, "Channel (num) must be 1 or 2.");
	return(1);
    }
    //fprintf(stdout, "Return from dev (double): \n %e, %e \n", val_out1, val_out2);
    //fprintf(stdout, "Return from dev (double): \n %lf \n", *val_out);
    return(0);
}



#include "main.h"

