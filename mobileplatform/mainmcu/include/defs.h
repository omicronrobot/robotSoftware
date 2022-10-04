#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>

#define DEBUG 2
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

// This is data used by the clients mobile and hand motion sensors
struct Data
{
    float pitch; // Rotation around the side-to-side axis
    float roll;  // Rotation around the front-to-back axis
    float yaw;   // Rotation around the vertical axis
};

#endif