/* Program for reading a custom made Arduino POE ADC  */
/* and putting said readings in to a mysql database. */
/* James Nikkel */
/* james.nikkel@rhul.ac.uk */
/* Copyright 2013 */
/* James public licence. */

#include "SC_db_interface.h"
#include "SC_aux_fns.h"
#include "SC_sensor_interface.h"

#include "v65xx.h"
#include "CAENComm.h"

// This is the default instrument entry, but can be changed on the command line when run manually.
// When called with the watchdog, a specific instrument is always given even if it is the same
// as the default. 
#define INSTNAME "CAEN_HV"

int inst_dev;

int get_reading(double *val, int channel, char *subtype)
{
  // 

   // Connection info: optical fiber over link #0, node #0, with VME address AAAA0000  
  int32_t  connectionType = CAENComm_OpticalLink;
  int32_t  link = 0;
  int32_t  conet = 0;
  uint32_t vme_address = 0xaaaa0000;
  int32_t  handle;
  
  CAENComm_ErrorCode  ret=0;
  
  // Open the HV module for communication
  ret = CAENComm_OpenDevice(connectionType, link, conet, vme_address, &handle);

  // Break if the HV module could not be opened
  if(ret < 0 ) {
    printf("Could not connect to the HV unit \n");
    CAENComm_CloseDevice(handle);
    return 1;
  }

  uint16_t data=0;
  uint8_t  ch;
  ch = uint8_t(channel);

  // Check whether board is in alarm state
  CAENComm_Read16(handle, 0x58, &data); // Alarm?
  fprintf(stdout, "Alarm=%d  ",data);
  
  if (type == 1)
    {
      CAENComm_Read16(handle, 0x88+(128*ch), &data) ;	// Read Vmon
      *val = (double)data / 10.0;
    }
  else if (type == 1)
    {
      CAENComm_Read16(handle, 0x8c+(128*ch), &data) ;	// Read ImonH
      *val = (double)data / 20.0;
    }
  else
    {
      fprintf(stderr, "type 1 for voltage, 2 for current");	
      return(1);
      }
  return(0);
}


#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  return(0);
}

#define _def_clean_up_inst
void clean_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)
{
  return(0);
}

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
  sleep(5);

  if (get_reading(*val_out, s_s->num, s_s->subtype))
    return(1);
    
  return(0);
}

#include "main.h"
