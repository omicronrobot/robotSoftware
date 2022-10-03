#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "mbed.h"
#include "math.h"

#define MAXIMUM_BUFFER_SIZE 18

// This is data used by the clients mobile and hand motion sensors
// For example 0200.1200.1000.h.m
struct Data
{
    int pitch;         // Rotation around the side-to-side axis. Int 0 to 20 i.e 02. Witch corresponds to -1.0 to +1.0
    int roll;          // Rotation around the front-to-back axis. Int 0 to 20 i.e 02. Witch corresponds to -1.0 to +1.0
    int yaw;           // Rotation around the vertical axis. Int 0 to 20 i.e 02. Witch corresponds to -1.0 to +1.0
    char publihser[1]; // The publisher of the message. It can be handmotion or mobile. String either h or m
    char protocol[1];  // The protocol used to send the message. It can be websocket, http or mqtt. String either w, h or m
};

float convert_to_float(int var)
{
    if (var == 0)
    {
        return -1.0;
    }
    else if (var == 10)
    {
        return 0.0;
    }
    else if (var == 20)
    {
        return 1.0;
    }
    else if (var < 10)
    {
        float num = ((float)(var)) * -0.1;
        return ((float)((int)(num * 10))) / 10;
    }
    else if (var > 10)
    {
        float num = ((float)(var - 10)) * 0.1;
        return ((float)((int)(num * 10))) / 10;
    }
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define DEBUG 1
#if DEBUG == 1
#define debug(x, ...) printf(x, ...)
#else
#define debug(x)
#endif
#endif