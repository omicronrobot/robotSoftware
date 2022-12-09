#ifndef MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_
#define MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_

#include "mbed.h"
#include <Defs.h>
#include <DRV8870.h>
#include <Stepper.h>

struct StepperPins
{
    PinName S1;
    PinName S2;
    PinName S3;
    PinName S4;

    // constructor
    StepperPins(PinName pin1, PinName pin2, PinName pin3, PinName pin4) : S1(pin1), S2(pin2), S3(pin3), S4(pin4){};
};

struct DCPins
{
    PinName ENA;
    PinName IN1;
    PinName IN2;

    // constructor
    DCPins(PinName pin1, PinName pin2, PinName pin3) : ENA(pin1), IN1(pin2), IN2(pin3){};
};

struct DrivePins
{
    DCPins TopLeft;
    DCPins TopRight;
    DCPins BottomLeft;
    DCPins BottomRight;

    // constructor
    DrivePins(DCPins pin1, DCPins pin2, DCPins pin3, DCPins pin4) : TopLeft(pin1), TopRight(pin2), BottomLeft(pin3), BottomRight(pin4){};
};

class Drive
{
private:

    Motor _top_left;
    Motor _top_right;
    Motor _bottom_left;
    Motor _bottom_right;
    Stepper _stepper;
    void _change_angle(uint16_t angle);
    void _change_speed(float speed);
    void _brake();
    int map(int x, int in_min, int in_max, int out_min, int out_max);

public:
    Drive(DrivePins drivePins, StepperPins stepperPins, uint16_t steps_per_rev, float step_delay);
    void init(float period);
    void drive(float pitch, uint16_t yaw);
};

#endif // MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_