#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "../include/imu.h"
#include "../include/wificonn.h"
#include "../include/mqttconn.h"
#include "../include/defs.h"

#define LED_BUILTIN 2

WiFiClient wclient;
SensorReadings sread;
WifiConn wificonn(wclient);
MQTTConn mqttconn(wclient);

void setup()
{

}

void loop()
{
  sread.Read();
  Data data = sread.GetData();
  mqttconn.SendData(data);
  // wait for a second
  delay(1000);
}

