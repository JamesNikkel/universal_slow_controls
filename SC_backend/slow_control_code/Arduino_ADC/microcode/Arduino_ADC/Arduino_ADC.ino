/*
 
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x66, 0xD8 };
IPAddress ip(192,168,1,90);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(5000);

void setup() 
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}


void loop() 
{
  int channel = -1;
  int reading;
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
          if (channel < 0 || channel > 15)
            client.println(-1);
          else
          {
            reading = analogRead(channel);
            client.println(reading);
          }
        }
        else 
          channel = c - '0';
      }
    }
    delay(10);
    // close the connection:
    client.stop();
  }
}

