#include "Drive.h"

Drive::Drive(DrivePins drivePins, StepperPins stepperPins, uint16_t steps_per_rev, uint16_t step_number, uint8_t stepper_direction) : _top_left(drivePins.TopLeft.IN1, drivePins.TopLeft.IN2), _top_right(drivePins.TopRight.IN1, drivePins.TopRight.IN2), _bottom_left(drivePins.BottomLeft.IN1, drivePins.BottomLeft.IN2), _bottom_right(drivePins.BottomRight.IN1, drivePins.BottomRight.IN2), _top_left_en(drivePins.TopLeft.ENA), _top_right_en(drivePins.TopRight.ENA), _bottom_left_en(drivePins.BottomLeft.ENA), _bottom_right_en(drivePins.BottomRight.ENA), _stepper(steps_per_rev, step_number, stepper_direction, stepperPins.S1, stepperPins.S2, stepperPins.S3, stepperPins.S4)
{
    this->_steps_per_rev = steps_per_rev;
}
void Drive::init(int stepperSpeed, float period)
{
    _top_left_en.write(1);
    _top_right_en.write(1);
    _bottom_left_en.write(1);
    _bottom_right_en.write(1);
    _top_left.setPeriod(period);
    _top_right.setPeriod(period);
    _bottom_left.setPeriod(period);
    _bottom_right.setPeriod(period);
    _stepper.setSpeed(stepperSpeed);
}
void Drive::drive(float pitch, uint16_t yaw)
{
    this->_change_speed(pitch);
    this->_change_direction(yaw);
}

void Drive::_change_direction(uint16_t direction)
{
    _stepper.step(this->map(direction, 0, 360, 0, _steps_per_rev));
}
void Drive::_change_speed(float speed)
{
    _top_left.setSpeed(speed);
    _top_right.setSpeed(speed);
    _bottom_left.setSpeed(speed);
    _bottom_right.setSpeed(speed);
}

void Drive::_brake()
{
    _top_left.brake(1);
    _top_right.brake(1);
    _bottom_left.brake(1);
    _bottom_right.brake(1);
}

int  Drive::map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}