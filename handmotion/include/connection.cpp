#include <WiFi101.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"
#include "connection.h"

Connection::Connection(WiFiClient client)
{
    wificlient = client;
    ConnectWiFi();
    SetupMQTT();
}

void Connection::Connect()
{
}

void Connection::Reconnect()
{
}

Connection::~Connection()
{
}

void Connection::ConnectWiFi()
{
    delay(1000);
    debugln();
    debug("Connecting to ");
    debugln(WLAN_SSID);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        debugln("WiFi shield not present");
        // don't continue:
        while (true)
            ;
    }

    // attempt to connect to WiFi network:
    while (wifistatus != WL_CONNECTED)
    {
        debug("Attempting to connect to WPA SSID: ");
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
    debugln("You're connected to the network");
    // printWifiData();
}

void Connection::printWiFiData()
{
    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    debug("IP address : ");
    debugln(ip);

    debug("Subnet mask: ");
    debugln((IPAddress)WiFi.subnetMask());

    debug("Gateway IP : ");
    debugln((IPAddress)WiFi.gatewayIP());
}

void Connection::SetupMQTT()
{
    // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
    Adafruit_MQTT_Client mqtt(&wificlient, MF_SERVER, MF_SERVERPORT, MF_USERNAME, MF_KEY);
    mqttclient = mqtt;
    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    ConnectMQTT();
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void Connection::ConnectMQTT()
{
    int8_t ret;
    if (mqttclient.connected())
    {
        return;
    }
    debug("Connecting to MQTT... ");

    uint8_t retries = 5;
    while ((ret = mqttclient.connect()) != 0)
    { // connect will return 0 for connected
        debugln(mqttclient.connectErrorString(ret));
        debugln("Retrying MQTT connection in 5 seconds...");
        mqttclient.disconnect();
        delay(5000); // wait 5 seconds
        retries--;
        if (retries == 0)
        {
            // basically die and wait for WDT to reset me
            while (1)
                ;
        }
    }
    debugln("MQTT Connected!");
}

Adafruit_MQTT_Client Connection::GetMQTTClient()
{
    return mqttclient;
}

void SendData(Data payload)
{
}