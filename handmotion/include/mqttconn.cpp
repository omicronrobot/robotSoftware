#include <WiFi101.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "defs.h"
#include "mqttconn.h"

MQTTConn::MQTTConn(WiFiClient client)
{
    wificlient = client;
    Connect();

    PUBTOPIC.toCharArray(PubBuf, 50);
    pubtopic = Adafruit_MQTT_Publish(&mqttclient, PubBuf);
}

void MQTTConn::Connect()
{
    // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
    Adafruit_MQTT_Client mqttclient(&wificlient, MF_SERVER, MF_SERVERPORT, MF_USERNAME, MF_KEY);

    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.

    int8_t ret;
    if (wificlient.connected())
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

void MQTTConn::Reconnect()
{
}

MQTTConn::~MQTTConn()
{
}

void MQTTConn::SendData(Data payload)
{
    char Cpayload[50];
    String Spayload = "Pitch" + String(payload.pitch) + "Roll" + String(payload.roll) + "Yaw" + String(payload.yaw) + "State" + String(payload.state);
    Spayload.toCharArray(Cpayload, 50);
    if (!pubtopic.publish(Cpayload))
    {
        debugln(F("Failed"));
    }
    else
    {
        debugln(F("OK!"));
    }
}