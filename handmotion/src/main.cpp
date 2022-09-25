#include "Arduino.h"
#include "../include/mpu6050.h"
#include "../include/connection.h"
#include "Adafruit_MQTT.h"

#define LED_BUILTIN 2

WiFiClient client;
SensorReadings sread;
Adafruit_MQTT_Publish pubtopic;
Connection conn(client);

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  // wait for a second
  delay(1000);
}

void SetupTopic(Adafruit_MQTT_Client mqtt)
{
  char PubBuf[50];
  PUBTOPIC.toCharArray(PubBuf, 50);
  pubtopic = Adafruit_MQTT_Publish(&mqtt, PubBuf);
}

void Connection::SendData(Data payload)
{
    if (!pubtopic.publish("payload"))
    {
        debugln(F("Failed"));
    }
    else
    {
        debugln(F("OK!"));
    }
}