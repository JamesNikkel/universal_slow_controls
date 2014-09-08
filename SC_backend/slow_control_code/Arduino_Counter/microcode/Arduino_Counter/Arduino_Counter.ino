/*
 
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x11, 0xAD, 0xBC, 0xFE, 0xEE, 0xED };
IPAddress ip(192,168,1,85);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(5000);

unsigned long count_A = 0;
unsigned long count_B = 0;


void countPulseA()
{
  count_A++;
}


void countPulseB()
{
  count_B++;
}


void setup() 
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  attachInterrupt(0, countPulseA, RISING);
  attachInterrupt(1, countPulseB, RISING);    
}


void loop() 
{
  int channel = -1;
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected())  
    {
      if (client.available()) 
      {
        char c = client.read();
        
        if (c == '\n') 
        {
          if (channel == 0)
            client.println(count_A);
          else if (channel == 1)
            client.println(count_B);
          else
            client.println("OK");
        }
        else if (c == 'r') 
        {
          count_A = 0;
          count_B = 0;
          channel = -1;
        }
        else if (c == '0') 
        {
          channel = 0;
        }
        else if (c == '1') 
        {
          channel = 1;
        }
      }
    }
    delay(10);
    // close the connection:
    client.stop();
  }
}

