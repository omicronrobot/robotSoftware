#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAXIMUM_BUFFER_SIZE 18

// This is data used by the clients mobile and hand motion sensors
// For example 0200.1200.1000.h.m
struct Data
{
    int pitch;         // Rotation around the side-to-side axis. Int 0 to 2000 i.e 0200. Witch corresponds to -1.00 to +1.00
    int roll;          // Rotation around the front-to-back axis. Int 0 to 2000 i.e 0200. Witch corresponds to -1.00 to +1.00
    int yaw;           // Rotation around the vertical axis. Int 0 to 2000 i.e 0200. Witch corresponds to -1.00 to +1.00
    char publihser[1]; // The publisher of the message. It can be handmotion or mobile. String either h or m
    char protocol[1];  // The protocol used to send the message. It can be websocket, http or mqtt. String either w, h or m
};
#endif