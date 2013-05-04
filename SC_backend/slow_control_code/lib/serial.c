/* This is for any additional code for serial interface comms. */
/* James Nikkel */
/* james.nikkel@yale.edu */
/* Copyright 2010 */
/* James public licence. */

#include "serial.h"

int query_serial(int fd, char *cmd_string, size_t c_count, char *ret_string, size_t r_count)
{
    int i;
    ssize_t wstatus, rdstatus;

    for (i = 0; i < MAX_SERIAL_RETRIES; i++)
    {
	wstatus = write(fd, cmd_string, c_count);
	msleep(200);
	rdstatus = read(fd, ret_string, r_count);
	
	if (rdstatus > 0)
	    return(0);
    }
    
    fprintf(stderr, "To many retries in query_serial. \n");
    return(1);
}

int write_serial(int fd, char *cmd_string, size_t c_count)
{
    int i;
    ssize_t wstatus;

    for (i = 0; i < MAX_SERIAL_RETRIES; i++)
    {
	wstatus = write(fd, cmd_string, c_count);
	
	if (wstatus > 0)
	    return(0);
    }
    
    fprintf(stderr, "To many retries in write_serial. \n");
    return(1);
}

int read_serial(int fd, char *ret_string, size_t r_count)
{
    int i;
    ssize_t rdstatus;

    for (i = 0; i < MAX_SERIAL_RETRIES; i++)
    {
	rdstatus = read(fd, ret_string, r_count);
	
	if (rdstatus > 0)
	    return(0);
    }
    
    fprintf(stderr, "To many retries in read_serial. \n");
    return(1);
}
