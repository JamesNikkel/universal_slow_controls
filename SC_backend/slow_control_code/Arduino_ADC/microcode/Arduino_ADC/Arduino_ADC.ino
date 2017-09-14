/*


*/
#include <stdint.h>
#include "SparkFunBME280.h"

#include "Wire.h"
#include "SPI.h"

#include <Ethernet.h>


BME280 pthSensor;
BME280 pthSensor_B;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xD4, 0x18
};
IPAddress ip(169, 254, 1, 97);

// Initialize the Ethernet server library
// with the IP address and port you want to use
EthernetServer server(5000);

char ret_string[128];
int  buff_pos = 0;

void setup()
{
  //pinMode(53, OUTPUT);
  //pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);


  pthSensor.settings.commInterface = I2C_MODE;
  pthSensor.settings.I2CAddress = 0x77;
  pthSensor.settings.runMode = 3;
  pthSensor.settings.tStandby = 0;

  pthSensor.settings.filter = 0;
  pthSensor.settings.tempOverSample = 1;
  pthSensor.settings.pressOverSample = 1;
  pthSensor.settings.humidOverSample = 1;

  delay(10);
  pthSensor.begin();
  delay(100);

  
  pthSensor_B.settings.commInterface = I2C_MODE;
  pthSensor_B.settings.I2CAddress = 0x76;
  pthSensor_B.settings.runMode = 3;
  pthSensor_B.settings.tStandby = 0;

  pthSensor_B.settings.filter = 0;
  pthSensor_B.settings.tempOverSample = 1;
  pthSensor_B.settings.pressOverSample = 1;
  pthSensor_B.settings.humidOverSample = 1;

  delay(10);
  pthSensor_B.begin();
  delay(100);

  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

}


void loop()
{
  unsigned int channel;
  int value = 0;

  boolean done_read = false;

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    char c = client.read();
    if (c == '\n')
    {
      ret_string[buff_pos] = '\0';
      done_read = true;
      buff_pos = 0;
    }
    else
    {
      ret_string[buff_pos] = c;
      buff_pos++;
    }

    if (done_read)
    {
      if (sscanf(ret_string, "RA %d", &channel) == 1)
      {
        if (channel < 0 || channel > 6)
          client.println(-1);
        else
        {
          value = analogRead(channel);
          client.println(value);
        }
      }
      else if (sscanf(ret_string, "RD %d", &channel) == 1)
      {
        if (channel < 2 || channel > 9)
          client.println(-1);
        else
        {
          pinMode(channel, INPUT);
          value = digitalRead(channel);
          client.println(value);
        }
      }
      
      else if (strncmp(ret_string, "RDT_B", 5) == 0)
      {
        client.println(pthSensor.readTempC());
      }
      else if (strncmp(ret_string, "RDP_B", 5) == 0)
      {
        client.println(pthSensor.readFloatPressure());
      }
      else if (strncmp(ret_string, "RDH_B", 5) == 0)
      {
        client.println(pthSensor.readFloatHumidity());
      }
      else if (strncmp(ret_string, "RDT", 3) == 0)
      {
        client.println(pthSensor.readTempC());
      }
      else if (strncmp(ret_string, "RDP", 3) == 0)
      {
        client.println(pthSensor.readFloatPressure());
      }
      else if (strncmp(ret_string, "RDH", 3) == 0)
      {
        client.println(pthSensor.readFloatHumidity());
      }
      else if (sscanf(ret_string, "WD %d %d", &channel, &value) == 2)
      {
        if (channel < 2 || channel > 9)
          client.println(-1);
        else
        {
          pinMode(channel, OUTPUT);
          if (value > 0)
            digitalWrite(channel, HIGH);
          else
            digitalWrite(channel, LOW);
        }
      }
    }

    //delay(10);
    // close the connection:
    //client.stop();
  }
}

