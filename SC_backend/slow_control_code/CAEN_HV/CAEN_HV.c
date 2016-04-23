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


#define _def_set_up_inst
int set_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)    
{
  return(0);
}

#define _def_clean_up_inst
void clean_up_inst(struct inst_struct *i_s, struct sensor_struct *s_s_a)
{
}

#define _def_read_sensor
int read_sensor(struct inst_struct *i_s, struct sensor_struct *s_s, double *val_out)
{
  sleep(4);
  
  int32_t  connectionType = CAENComm_OpticalLink;
  int32_t  link = 0;
  int32_t  conet = 0;
  uint32_t vme_address = 0xaaaa0000;
  int32_t  handle;
  uint16_t data=0;
  uint8_t  ch;

  CAENComm_ErrorCode  ret=0;
  
  // Open the HV module for communication
  ret = CAENComm_OpenDevice(connectionType, link, conet, vme_address, &handle);

  // Break if the HV module could not be opened
  if(ret < 0 ) {
    fprintf(stderr, "Could not connect to the HV unit ($s)\n", i_s->name);
    CAENComm_CloseDevice(handle);
    return(1);
  }

  ch = (uint8_t)s_s->num;   // get the channel number from the sensor structure

  // Check whether board is in alarm state
  CAENComm_Read16(handle, 0x58, &data); // Alarm?
  fprintf(stdout, "Alarm=%d  ",data);
  
  if (strncmp(s_s->subtype, "Vmon", 1) == 0)  // get the specific data from the sensor structure
    {
      CAENComm_Read16(handle, 0x88+(128*ch), &data) ;	// Read Vmon
      *val_out = (double)data / 10.0;
    }
  else if (strncmp(s_s->subtype, "ImonH", 1) == 0) 
    {
      CAENComm_Read16(handle, 0x8c+(128*ch), &data) ;	// Read ImonH
      *val_out = (double)data / 20.0;
    }
  else if (strncmp(s_s->subtype, "ImonL", 1) == 0) 
    {
      CAENComm_Read16(handle, 0xb8+(128*ch), &data) ;	// Read ImonL
      *val_out = (double)data / 20.0;
    }
  else if (strncmp(s_s->subtype, "Temp", 1) == 0) 
    {
      CAENComm_Read16(handle, 0xb0+(128*ch), &data) ;	// Read temp
      *val_out = (double)data;
    }
  else if (strncmp(s_s->subtype, "Status", 1) == 0) 
    {
      CAENComm_Read16(handle, 0x94+(128*ch), &data) ;	// Read Ch Status
      *val_out = (double)data;
    }
  else if (strncmp(s_s->subtype, "Power", 1) == 0) 
    {
      CAENComm_Read16(handle, 0x90+(128*ch), &data) ;	// Read Power
      *val_out = (double)data;
    }
  else
    {
      fprintf(stderr, "No such subtype in %s. \n", i_s->name);	
      CAENComm_CloseDevice(handle);
      return(1);
    }
  
  CAENComm_CloseDevice(handle);  
  return(0);
}

#include "main.h"
