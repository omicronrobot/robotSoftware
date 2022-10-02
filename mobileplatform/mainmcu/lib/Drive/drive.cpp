#include "drive.h"
#include <DRV8870.h>

Drive::Drive(DrivePins drivePins, StepperPins stepperPins, int steps_per_rev) : _top_left(drivePins.TopLeft.IN1, drivePins.TopLeft.IN2), _top_right(drivePins.TopRight.IN1, drivePins.TopRight.IN2), _bottom_left(drivePins.BottomLeft.IN1, drivePins.BottomLeft.IN2), _bottom_right(drivePins.BottomRight.IN1, drivePins.BottomRight.IN2), _top_left_en(drivePins.TopLeft.ENA), _top_right_en(drivePins.TopRight.ENA), _bottom_left_en(drivePins.BottomLeft.ENA), _bottom_right_en(drivePins.BottomRight.ENA), _stepper(steps_per_rev, stepperPins.S1, stepperPins.S2, stepperPins.S3, stepperPins.S4)
{
    _steps_per_rev = steps_per_rev;
}
void Drive::init(int stepperSpeed)
{
    _top_left_en.write(1);
    _top_right_en.write(1);
    _bottom_left_en.write(1);
    _bottom_right_en.write(1);
    _top_left.period(0.01);
    _top_right.period(0.01);
    _bottom_left.period(0.01);
    _bottom_right.period(0.01);
	_stepper.setSpeed(stepperSpeed);
}
void Drive::drive(Data data)
{
    if (data.roll != 10)
    {
        this->_change_speed(data);
    }
    else
    {
    }
}

void Drive::_change_direction(Data data)
{
    _stepper.step(map(data.yaw, 0, 20, 0, _steps_per_rev));
}
void Drive::_change_speed(Data data) {
    _top_left.speed(convert_to_float(data.roll));
    _top_right.speed(convert_to_float(data.roll));
    _bottom_left.speed(convert_to_float(data.roll));
    _bottom_right.speed(convert_to_float(data.roll));
}

void Drive::_brake()
{
    _top_left.brake(1);
    _top_right.brake(1);
    _bottom_left.brake(1);
    _bottom_right.brake(1);
}