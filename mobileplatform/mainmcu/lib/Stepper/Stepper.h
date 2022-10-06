/*
 * Drives a unipolar, bipolar, or five phase stepper motor.
 *
 * When wiring multiple stepper motors to a microcontroller, you quickly run
 * out of output pins, with each motor requiring 4 connections.
 *
 * By making use of the fact that at any time two of the four motor coils are
 * the inverse of the other two, the number of control connections can be
 * reduced from 4 to 2 for the unipolar and bipolar motors.
 *
 * A slightly modified circuit around a Darlington transistor array or an
 * L293 H-bridge connects to only 2 microcontroller pins, inverts the signals
 * received, and delivers the 4 (2 plus 2 inverted ones) output signals
 * required for driving a stepper motor. Similarly the Arduino motor shield's
 * 2 direction pins may be used.
 *
 *
 * The sequence of control signals for 4 control wires is as follows:
 *
 * Step C0 C1 C2 C3
 *    1  1  0  1  0
 *    2  0  1  1  0
 *    3  0  1  0  1
 *    4  1  0  0  1
 */

#ifndef MOBILEPLATFORM_MAINMCU_LIB_STEPPER_STEPPER_H_
#define MOBILEPLATFORM_MAINMCU_LIB_STEPPER_STEPPER_H_

#include "mbed.h"

class Stepper
{
private:
    // motor pin numbers:
    DigitalOut _motor_pin_1;
    DigitalOut _motor_pin_2;
    DigitalOut _motor_pin_3;
    DigitalOut _motor_pin_4;
    int _direction;                // Direction of rotation
    unsigned u_int64_t _step_delay;     // delay between steps, in us, based on speed
    int _number_of_steps;          // total number of steps this motor can take
    int _step_number;              // which step the motor is on
    unsigned u_int64_t _last_step_time; // timestamp in us of when the last step was taken
    void stepMotor(int this_step);

public:
    Stepper(int number_of_steps, PinName motor_pin_1, PinName motor_pin_2, PinName motor_pin_3, PinName motor_pin_4);

    void setSpeed(u_int16_t whatSpeed);
    void step(int number_of_steps);
};

#endif // MOBILEPLATFORM_MAINMCU_LIB_STEPPER_STEPPER_H_
