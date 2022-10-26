#ifndef MOBILEPLATFORM_MAINMCU_LIB_DEFS_H_
#define MOBILEPLATFORM_MAINMCU_LIB_DEFS_H_

#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 18

// This is data used by the clients mobile and hand motion sensors
// For example
// {
//   "pitch": 57.992,
//   "yaw": 57992,
//   "metadata": "handmotion"
// }
struct Data
{
    float pitch;    // The pitch for speed control of the robot. Speed is either max forward or zero or max revers. float -1 to +1.
    uint16_t yaw;   // Yaw used to control the direction of the robot. Rotation about perpendicular axis to the floor. Int 0 to 360.
    char *metadata; // The metadata of the message. It can be used to add any payload to the message for example the publisher.
};

#define DEBUG 1
#if DEBUG == 1
#define debug(x, ...) printf(x, ...)
#else
#define debug(x)
#endif
#endif // MOBILEPLATFORM_MAINMCU_LIB_DEFS_H_