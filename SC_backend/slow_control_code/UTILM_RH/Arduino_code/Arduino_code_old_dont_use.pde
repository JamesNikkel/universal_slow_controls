
/*
  Web Server
  Krishna Mooroogen
  21st June 2012 
  web server for capacitance liquid level sensor 
  MINIclean 
  Supervisor Dr James Nikkel 
  controller to be able to talk via ethernet to a master program
*/

#include <SPI.h>
#include <Ethernet.h>//calls ethernet library 
  
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xE7, 0xD };
byte ip[] = {192,168,1,177};// sets ip and mac address 

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(5000);//access port 

int Low = 0;//definig global variable for the mode selector 
int High = 1;
int mode_select = High;// This is the default mode behaviour 


void set_UTI_mode()
/*mode selector function, <2pf low mode or <300pf high mode, both 3 capacitors modes, enables ouput pins to control 
  weather the selectors are on or off refer to UTI manual for other modes and selector pins
*/

{
  if (mode_select == Low)//if set to low then, mode low all other cases mode is high, i.e default 
    {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(6,LOW);
      digitalWrite(5,HIGH);
    }
    
  else
       
    {
      digitalWrite(2,LOW);
      digitalWrite(3,HIGH);
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);
    }
}
  
  
int ReadUTI(float *cap) 
/*funtion readUTI, turns uti chip on, takes times using the micros function(returns time in micro seconds that arduino has been running) on the rising and falling edges of the 3 piece signal, 
  idetifies each part and assigns them accordingly as toff, tx and tref reffering to the UTI manual.
*/
{
  int capREF = 100;
  int b = 0; //define array intergers 
  int j;
  int i;
  unsigned long width[34] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //define array for 'time stamps' need 34 as we need 16 periods, as i=i+2 
  unsigned long timep[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//array for storing periods 
  float toff = 0;
  float tref = 0;
  float tx = 0; //define periods 
  float time1 = 0;
  float time2 = 0;
  float time = 0;  
  int   input_pin = 8;
 
  //defines used variables 
  digitalWrite(7,HIGH);
  delay(2000);//turns chip on

  for(i = 0;i < 33; i += 2) //for these iterations moving in twos, so that in each turn two slots are filled. 
    { 
      while (digitalRead(input_pin) == LOW)
	{
	  width[i]=micros(); //takes time at rising edge (will continuley take time stamps while high or low, will overwrite with the last value making it the rising or fallign edge) time program has been running at that point. 
        }
      
      while  (digitalRead(input_pin) == HIGH)
	{
	  width[i+1]=micros(); //takes time at falling edge, first reading will be unusable from this method 
	}
    }  
    
//  if (width[0] > width[32])
//    return(0);
  
  digitalWrite(7,LOW);//turns UTI off
  
  j = 0;
  for(i = 0; i < 32; i += 2)
    {
      timep[j]=width[i+2]-width[i]; //calculates the time periods, time the board has been running minus previous time, stores in array timep 
      j++;
    }
    

  for(i = 0; i < 7; i++) 
    {
      if ((0.95*timep[i+1])<=timep[i]<=(1.05*timep[i+1])&&timep[i]<timep[i+2]&&timep[i]<timep[i+3]&&timep[i+1]<timep[i+2]&&timep[i+1]<timep[i+3])
	//selects the starting position by defining the first peroid as being the first two smallest periods which lie within 5% of each other
 
        {  
	  //picking the offset periods 
	  toff=timep[i]+timep[i+1];
	  tref=timep[i+2];
	  tx=timep[i+3];
 
	  *cap=((tx-toff)/(tref-toff))*capREF; //UTI 3 signal calculation  
//          return(1);
	}
    }
  return(1);
}

 
float read_cap()//fucntion readd cap, averages runs and returns value. 
{    
  //  Reads out and averages the capacitance value
  float cap; //definine variables for later
  float cap_sum = 0;
  int   i = 0;
  int   max_averages = 10; //loop ten times for an average capacitance 
  int   actual_averages = 0;

  for (i = 0; i < max_averages; i++) 
    {                  
      if (ReadUTI(&cap))    //fucntion defined above
	{  
	  cap_sum += cap;
	  actual_averages++;
	  delay(500);         //wait periods to stop clogging 
	}
    }

  if (actual_averages > 0)
    return(cap_sum/actual_averages);//only divides by the number of succesful runs
  else
    return(0);
}  
 
  
    

void setup() //function set initial set up
{
  pinMode (2, OUTPUT);   //set pins 2-6 as outputs, 7 is on and off pin 
  pinMode (3, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (8, INPUT);     //input signal
   
  set_UTI_mode();  // Will set mode to default

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}

void loop() 
{
  // listen for incoming clients
  
  char ret_string[100];
  boolean done_read = false;
  char mode_string[16];
  int capacitance;
  int i=0;
  
  EthernetClient client = server.available();
  
  if (client) 
    {
      char c = client.read();
         
      if (c == '\n') 
	{ 
	  ret_string[i]='\0';
	  done_read = true;
	  i=0;
	  Serial.print("ret_string print:");
	  Serial.print(ret_string);
	  Serial.print("\n");
	}  
      else 
	{
	  ret_string[i]=c;
	  i++;
	}
	    
      if (done_read)
	{
	  if (strncmp(ret_string,"GetCap", 6) == 0)//string comapre from client 
	    {
	      capacitance=(int)(read_cap()*1000000);
	      client.println(capacitance);//return average capacitance value 
	      Serial.print(capacitance);
	    }
  
	  else if (strncmp(ret_string,"SetMode High", 12) == 0)
	    {
	      mode_select=High;//string compare mode selector 
	      set_UTI_mode();//refers back to mode slector function 
	    }

	  else if (strncmp(ret_string,"SetMode Low", 11) == 0)
	    {
	      mode_select=Low;
	      set_UTI_mode();
	    }
	}
    }
}
