#include "Drive.h"

Drive::Drive(DrivePins drivePins, StepperPins stepperPins, uint16_t steps_per_rev) : _top_left(drivePins.TopLeft.IN1, drivePins.TopLeft.IN2), _top_right(drivePins.TopRight.IN1, drivePins.TopRight.IN2), _bottom_left(drivePins.BottomLeft.IN1, drivePins.BottomLeft.IN2), _bottom_right(drivePins.BottomRight.IN1, drivePins.BottomRight.IN2), _stepper(steps_per_rev, stepperPins.S1, stepperPins.S2, stepperPins.S3, stepperPins.S4)
{
    this->_steps_per_rev = steps_per_rev;
    DigitalOut _top_left_en(drivePins.TopLeft.ENA);
    DigitalOut _top_right_en(drivePins.TopRight.ENA);
    DigitalOut _bottom_left_en(drivePins.BottomLeft.ENA);
    DigitalOut _bottom_right_en(drivePins.BottomRight.ENA);
    _top_left_en = 1;
    _top_right_en = 1;
    _bottom_left_en = 1;
    _bottom_right_en = 1;
}
void Drive::init(int stepperSpeed, float period)
{
    this->_top_left.setPeriod(period);
    this->_top_right.setPeriod(period);
    this->_bottom_left.setPeriod(period);
    this->_bottom_right.setPeriod(period);
    this->_stepper.setSpeed(stepperSpeed);
}
// We rotate 180. If we want to drive reverse we change the speed to negative
void Drive::drive(float pitch, uint16_t yaw)
{
    if ((yaw >= 270) && (yaw <= 360))
    {
        if (pitch <= 0)
        {
            this->_change_speed(pitch);
            this->_change_direction(yaw - 180);
        }
        else
        {
            this->_change_speed(pitch * -1);
            this->_change_direction(yaw - 180);
        }
    }
    else
    {
        this->_change_speed(pitch);
        this->_change_direction(yaw);
    }
}

void Drive::_change_direction(uint16_t direction)
{
    this->_stepper.step(this->map(direction, 0, 360, 0, _steps_per_rev));
}
void Drive::_change_speed(float speed)
{
    this->_top_left.setSpeed(speed);
    this->_top_right.setSpeed(speed);
    this->_bottom_left.setSpeed(speed);
    this->_bottom_right.setSpeed(speed);
}

void Drive::_brake()
{
    this->_top_left.brake(1);
    this->_top_right.brake(1);
    this->_bottom_left.brake(1);
    this->_bottom_right.brake(1);
}

int Drive::map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}