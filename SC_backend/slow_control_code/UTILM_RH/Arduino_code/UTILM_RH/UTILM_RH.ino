/*
  Read out of UTI chip in 3 capacitor mode using 
  Arduino ethernet.
*/

#include <SPI.h>
#include <Ethernet.h>

 
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xE7, 0xD1 };
byte ip[] = {192,168,1,177};

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(5000);

char ret_string[128];
int  buff_pos = 0;

//definig global variable for the UTI mode selector 
int Low_mode  = 1;  
int High_mode = 4;

// Define Arduino Pins:
int SEL_1    = 2;
int SEL_2    = 3;
int SEL_3    = 5;
int SEL_4    = 6;

int UTI_PWR  = 7;
int UTI_OUT  = 8;

int LED      = 9;

void SetUTImode(int mode_select)
{
  /* mode selector function, <2pf low mode or <300pf high mode, both 3 capacitors modes, 
     enables ouput pins to control whether the selectors are on or off refer to UTI 
     manual for other modes and selector pins
  */
  if (mode_select == 0)      // 5 cap, 0-2 pF
    {
      digitalWrite(SEL_1, LOW );
      digitalWrite(SEL_2, LOW );
      digitalWrite(SEL_3, LOW );
      digitalWrite(SEL_4, LOW);
    }  
  else if (mode_select == 1)  // 3 cap, 0-2 pF
    {
      digitalWrite(SEL_1, LOW );
      digitalWrite(SEL_2, LOW );
      digitalWrite(SEL_3, LOW );
      digitalWrite(SEL_4, HIGH);
    }
  else if (mode_select == 2)  // 5 cap, 0-12 pF
    {
      digitalWrite(SEL_1, LOW );
      digitalWrite(SEL_2, LOW );
      digitalWrite(SEL_3, HIGH);
      digitalWrite(SEL_4, LOW );
    }
  else if (mode_select == 3)  // External MUX, 0-2 pF
    {
      digitalWrite(SEL_1, LOW );
      digitalWrite(SEL_2, LOW );
      digitalWrite(SEL_3, HIGH);
      digitalWrite(SEL_4, HIGH);
    }
  else if (mode_select == 4)   // 3 cap, 0-300 pF
    {
      digitalWrite(SEL_1, LOW );
      digitalWrite(SEL_2, HIGH);
      digitalWrite(SEL_3, LOW );
      digitalWrite(SEL_4, LOW );
    }
}

void TurnOnUTI(void)
{
  digitalWrite(UTI_PWR, HIGH);
  delay(1000);
}
  
void TurnOffUTI(void)
{
  digitalWrite(UTI_PWR, LOW);
}


int ReadUTI(float *cap) 
{
  /*  Turns UTI chip on, takes times using the 'micros' function 
      on the rising and falling edges of the 3 piece signal, 
      idetifies each part and assigns them accordingly as 
      toff, tx and tref reffering to the UTI manual.
      Sets 'cap' to a float corresponding to the calculated capacitance in pF. 
      Returns 1 if good, 0 if there is some error.
  */
  float capREF = 2.2;
  int b = 0; //define array intergers 
  int j;
  int i;
  //define array for 'time stamps' need 34 as we need 16 periods, as i=i+2:
  unsigned long width[34] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  unsigned long timep[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//array for storing periods 
  float toff = 0;
  float tref = 0;
  float tx = 0; //define periods 
  float time1 = 0;
  float time2 = 0;
  float time = 0;  
  
  //for these iterations moving in twos, so that in each turn two slots are filled. 
  for(i = 0;i < 33; i += 2) 
    { 
      while (digitalRead(UTI_OUT) == LOW)
	{
	  // takes time at rising edge (will continuley take time stamps while high or low, 
	  // will overwrite with the last value making it the rising or falling edge) 
	  // time program has been running at that point. 
	  width[i]=micros(); 
        }
      
      while  (digitalRead(UTI_OUT) == HIGH)
	{
	  // takes time at falling edge, first reading will be unusable 
	  // from this method (changed from micros to test)
	  width[i+1]=micros();
	}
    }  
    
  if (width[0] > width[32])
    return(0);
    
  j = 0;
  for(i = 0; i < 32; i += 2)
    {
      // calculates the time periods, time the board has been 
      // running minus previous time, stored in array timep 
      timep[j]=width[i+2]-width[i]; 
      j++;
    }
    
  for(i = 0; i < 8; i++) 
    {
      if ( (0.95*timep[i+1])<=timep[i]  &&
           timep[i]<=(1.05*timep[i+1])  &&
           timep[i]<timep[i+2]          &&
           timep[i]<timep[i+3]          &&
           timep[i+1]<timep[i+2]        &&
           timep[i+1]<timep[i+3])
	// selects the starting position by defining the first peroid as 
	// being the first two smallest periods which lie within 5% of each other
        {  
	  //picking the offset periods 
	  toff=timep[i]+timep[i+1];
	  tref=timep[i+2];
	  tx=timep[i+3];
 
	  *cap=((tx-toff)/(tref-toff))*capREF; //UTI 3 signal calculation  
	  return(1);
	}
    }
  return(0);
}

float ReadCap()
{    
  //  Reads out and averages the capacitance value
  //  and returns the capacitance in pF.
  float cap = 0; 
  float cap_sum = 0;
  int   i;
  int   max_averages = 10; // Loop five times for an average capacitance 
  int   actual_averages = 0;

  TurnOnUTI();
  for (i = 0; i < max_averages; i++) 
    {                  
      if (ReadUTI(&cap))    //function defined above
	{  
          cap_sum += cap;
          actual_averages++;
          delay(10);         //wait periods to stop clogging 
	}
    }
  TurnOffUTI();
  
  if (actual_averages > 0)
    return(cap_sum/actual_averages);//only divides by the number of succesful runs
  else
    return(-1);
}  


void setup()
{
  pinMode (SEL_1,   OUTPUT); 
  pinMode (SEL_2,   OUTPUT);
  pinMode (SEL_3,   OUTPUT);
  pinMode (SEL_4,   OUTPUT);
  pinMode (UTI_PWR, OUTPUT);
  pinMode (UTI_OUT, INPUT);   
  pinMode (LED,     OUTPUT);  
  delay(1000);
  
  digitalWrite(LED,      LOW);
  digitalWrite(UTI_PWR, LOW);

  SetUTImode(High_mode); // Deafault to High mode.

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}


void loop() 
{
  // listen for incoming clients
  
  boolean done_read = false;
  char mode_string[16];
  long capacitance;
  float capa;

  EthernetClient client = server.available();
  
  if (client) 
    {
      char c = client.read();
         
      if (c == '\n') 
	{ 
	  ret_string[buff_pos]='\0';
	  done_read = true;
	  buff_pos=0;
	}  
      else 
	{
	  ret_string[buff_pos]=c;
	  buff_pos++;
	}
	    
      if (done_read)
	{
	  if (strncmp(ret_string,"Get", 3) == 0)
	    {
              digitalWrite(LED, HIGH);
              capa=(ReadCap());
	      capacitance=(long)(capa*1000000.0);
	      client.println(capacitance);
              digitalWrite(LED, LOW);  
	    }
  
	  else if (strncmp(ret_string,"SetMode High", 12) == 0)
	      SetUTImode(High_mode);

	  else if (strncmp(ret_string,"SetMode Low", 11) == 0)
	      SetUTImode(Low_mode);
	}
    }
}


