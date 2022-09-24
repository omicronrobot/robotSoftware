
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <WiFi101.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"

WiFiClient client;

void ConnectWifi()
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
    printWiFiData();
}

void printWiFiData()
{
    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    debug("IP address : ");
    debugln(ip);

    debug("Subnet mask: ");
    debugln((IPAddress)WiFi.subnetMask());

    debug("Gateway IP : ");
    debugln((IPAddress)WiFi.gatewayIP());

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    debug("MAC address: ");
    printMacAddress(mac);
}


void printMacAddress(byte mac[])
{
    for (int i = 5; i >= 0; i--)
    {
        if (mac[i] < 16)
        {
            debug("0");
        }
        debug(mac[i], HEX);
        if (i > 0)
        {
            debug(":");
        }
    }
    debugln();
}

void SetupMQTT()
{
    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    MQTT_connect();

    // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
    Adafruit_MQTT_Client mqtt(&client, MF_SERVER, MF_SERVERPORT, MF_USERNAME, MF_KEY);

    ptopic = "channels/" + CHANNEL_ID + "/messages" Adafruit_MQTT_Publish timefeed = Adafruit_MQTT_Subscribe(&mqtt, ptopic);
    stopic = "channels/" + CHANNEL_ID + "/messages/subscribe" Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, stopic);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
    int8_t ret;
    if (mqtt.connected())
    {
        return;
    }
    debug("Connecting to MQTT... ");

    uint8_t retries = 5;
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
        debugln(mqtt.connectErrorString(ret));
        debugln("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
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

void SendMQTTData(Data payload)
{
    // Now we can publish stuff!
    debug("\nSending photocell val ");
    debug(payload);
    debug("...");
    if (!photocell.publish(x++))
    {
        debugln(F("Failed"));
    }
    else
    {
        debugln(F("OK!"));
    }
}

#endif