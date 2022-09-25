#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"
#include "wificonn.h"

WifiConn::WifiConn(WiFiClient client)
{
    wificlient = client;
    Connect();
}

void WifiConn::Reconnect()
{
}

WiFiClient WifiConn::GetClient()
{
    return wificlient;
}

WifiConn::~WifiConn()
{
    // if the server's disconnected, stop the client:
    if (!wificlient.connected())
    {
        debugln("DISCONNECTING FROM SERVER.");
        wificlient.stop();

        // do nothing forevermore:
        while (true)
            ;
    }
}

void WifiConn::Connect()
{
    delay(1000);
    debug("CONNECTING TO: ");
    debugln(WLAN_SSID);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        debugln("WIFI NOT PRESENT");
        // don't continue:
        while (true)
            ;
    }

    // attempt to connect to WiFi network:
    while (wifistatus != WL_CONNECTED)
    {
        debug("ATTEMPTING TO CONNECT TO WPA SSID: ");
        debugln(ssid);
        // Connect to WPA/WPA2 network:
        wifistatus = WiFi.begin(ssid, pass);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            debug(".");
        }
    }
    // you're connected now, so print out the data:
    debugln("YOU'RE CONNECTED");
    printWiFiData();
}

void WifiConn::printWiFiData()
{
    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    debug("IP ADDRESS : ");
    debugln(ip);

    debug("SUBNET MASK: ");
    debugln((IPAddress)WiFi.subnetMask());

    debug("GATEWAY IP : ");
    debugln((IPAddress)WiFi.gatewayIP());
}
