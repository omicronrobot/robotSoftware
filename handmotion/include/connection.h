#ifndef CONNECTION_H
#define CONNECTION_H
#include <WiFi101.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"

class Connection
{
private:
    void ConnectWiFi();
    void printWiFiData();
    void ConnectMQTT();
    void SetupMQTT();

public:
    Connection(WiFiClient client);
    ~Connection();
    Adafruit_MQTT_Client GetMQTTClient();
    void Connect();
    void Reconnect();
    void SendData(Data payload);
    WiFiClient wificlient;
};

#endif