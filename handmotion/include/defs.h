#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>
#include <WiFi101.h>

#define WLAN_SSID "WIFI_SSID"
#define WLAN_PASS "WIFI_PASSWORD"

#define MF_SERVER "http://omicronrobot.rodneyosodo.com"
#define MF_SERVERPORT 1883
#define MF_USERNAME "TO_SETUP"
#define MF_KEY "TO_SETUP"

char ssid[] = WLAN_SSID;         // your network SSID (name)
char pass[] = WLAN_PASS;         // your network password (use for WPA, or use as key for WEP)
int wifistatus = WL_IDLE_STATUS; // the WiFi radio's status

#define DEBUG 2
#if DEBUG == 1
// #define debug(x) Serial.print(x)
// #define debugln(x) Serial.println(x)
// #define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

// This is data used by the handmotion sensors
// It contains the pitch used for forward and backward motion
// Roll used for sideways motion
// Yaw used to steering purposes
// State is the state of the application
struct Data
{
    float pitch; // Rotation around the side-to-side axis
    float roll;  // Rotation around the front-to-back axis
    float yaw;   // Rotation around the vertical axis
    int state;
};

#endif