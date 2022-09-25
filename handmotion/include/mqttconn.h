#ifndef MQTTCONN_H
#define MQTTCONN_H
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"

class MQTTConn
{
private:
    // Data
    char PubBuf[50];
    WiFiClient wificlient;
    Adafruit_MQTT_Client mqttclient = Adafruit_MQTT_Client(&wificlient, MF_SERVER, MF_SERVERPORT, MF_USERNAME, MF_KEY);
    Adafruit_MQTT_Publish pubtopic = Adafruit_MQTT_Publish(&mqttclient, PubBuf);
    // Functions
    void Connect();

public:
    MQTTConn(WiFiClient client);
    void Reconnect();
    void SendData(Data payload);
    ~MQTTConn();
};

#endif