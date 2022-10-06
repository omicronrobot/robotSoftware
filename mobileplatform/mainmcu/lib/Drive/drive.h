#ifndef MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_
#define MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_

#include "mbed.h"
#include "defs.h"
#include <DRV8870.h>
#include <Stepper.h>

struct StepperPins
{
    PinName S1;
    PinName S2;
    PinName S3;
    PinName S4;
};

struct DPins
{
    PinName ENA;
    PinName IN1;
    PinName IN2;
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
    DigitalOut _top_left_en;
    DigitalOut _top_right_en;
    DigitalOut _bottom_left_en;
    DigitalOut _bottom_right_en;
    Motor _top_left;
    Motor _top_right;
    Motor _bottom_left;
    Motor _bottom_right;
    int _steps_per_rev;
    Stepper _stepper;
    void _change_direction(Data data);
    void _change_speed(Data data);
    void _brake();

public:
    Drive(DrivePins drivePins, StepperPins stepperPins, int steps);
    void init(int stepperSpeed);
    void drive(Data data);
};

#endif // MOBILEPLATFORM_MAINMCU_LIB_DRIVE_DRIVE_H_