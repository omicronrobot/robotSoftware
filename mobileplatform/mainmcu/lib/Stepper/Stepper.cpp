#include "Stepper.h"

Stepper::Stepper(int number_of_steps, float delay_time, PinName motor_pin_1, PinName motor_pin_2, PinName motor_pin_3, PinName motor_pin_4) : _motor_pin_1(motor_pin_1), _motor_pin_2(motor_pin_2), _motor_pin_3(motor_pin_3), _motor_pin_4(motor_pin_4)
{
    this->time_delay = delay_time;
    this->no_steps = number_of_steps;
}

void Stepper::stepperDrive(int angle)
{
    if (angle < 180)
    {
        this->forward(this->stepper_map(angle, 0, 180, 0, this->no_steps));
    }
    if (angle > 180)
    {
        this->reverse(this->stepper_map(angle - 180, 0, 180, 0, this->no_steps));
    }
    this->previous_step = angle;
}

void Stepper::reverse(int steps)
{
    while (1)
    {
        this->_motor_pin_1 = 0;
        this->_motor_pin_2 = 1;
        this->_motor_pin_3 = 1;
        this->_motor_pin_4 = 0;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 0;
        this->_motor_pin_2 = 1;
        this->_motor_pin_3 = 0;
        this->_motor_pin_4 = 1;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 1;
        this->_motor_pin_2 = 0;
        this->_motor_pin_3 = 0;
        this->_motor_pin_4 = 1;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 1;
        this->_motor_pin_2 = 0;
        this->_motor_pin_3 = 1;
        this->_motor_pin_4 = 0;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
    }
}

void Stepper::forward(int steps)
{
    while (1)
    {
        this->_motor_pin_1 = 1;
        this->_motor_pin_2 = 0;
        this->_motor_pin_3 = 1;
        this->_motor_pin_4 = 0;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 1;
        this->_motor_pin_2 = 0;
        this->_motor_pin_3 = 0;
        this->_motor_pin_4 = 1;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 0;
        this->_motor_pin_2 = 1;
        this->_motor_pin_3 = 0;
        this->_motor_pin_4 = 1;
        wait(time_delay);
        steps--;
        if (steps < 1)
            break;
        this->_motor_pin_1 = 0;
        this->_motor_pin_2 = 1;
        this->_motor_pin_3 = 1;
        this->_motor_pin_4 = 0;
        wait(this->time_delay);
        steps--;
        if (steps < 1)
            break;
    }
}

int Stepper::stepper_map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}