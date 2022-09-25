#ifndef WIFICONN_H
#define WIFICONN_H
#include <ESP8266WiFi.h>
#include "defs.h"

class WifiConn
{
private:
    // Data
    WiFiClient wificlient;

    // Functions
    void Connect();
    void printWiFiData();

public:
    WifiConn(WiFiClient client);
    void Reconnect();
    WiFiClient GetClient();
    ~WifiConn();
};

#endif