#ifndef UART_H
#define UART_H

#include "defs.h"

struct DPins
{
    int ENA;
    int IN1;
    int IN2;
};

struct DrivePins
{
    DPins TopLeft;
    DPins TopRight;
    DPins BottomLeft;
    DPins BottomRight;
};

class Drive
{
private:
    /* data */
    DrivePins drive_pins;

public:
    Drive(/* args */);
    void init(DrivePins drivePins);
    void drive(Data data);
    ~Drive();
};

#endif