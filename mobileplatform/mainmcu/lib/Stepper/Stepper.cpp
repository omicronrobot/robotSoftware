#include "mbed.h"
#include "Stepper.h"

Stepper::Stepper(int number_of_steps, PinName motor_pin_1, PinName motor_pin_2, PinName motor_pin_3, PinName motor_pin_4) : _motor_pin_1(motor_pin_1), _motor_pin_2(motor_pin_2), _motor_pin_3(motor_pin_3), _motor_pin_4(motor_pin_4)
{
  this->_step_number = 0;                   // which step the motor is on
  this->_direction = 0;                     // motor direction
  this->_last_step_time = 0;                // timestamp in us of the last step taken
  this->_number_of_steps = number_of_steps; // total number of steps for this motor
}

void Stepper::setSpeed(u_int16_t whatSpeed)
{
  this->_step_delay = 60L * 1000L * 1000L / this->_number_of_steps / whatSpeed;
}

void Stepper::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move); // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0)
  {
    this->_direction = 1;
  }
  if (steps_to_move < 0)
  {
    this->_direction = 0;
  }

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned u_int64_t now = us_ticker_read();
    // move only if the appropriate delay has passed:
    if (now - this->_last_step_time >= this->_step_delay)
    {
      // get the timeStamp of when you stepped:
      this->_last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->_direction == 1)
      {
        this->_step_number++;
        if (this->_step_number == this->_number_of_steps)
        {
          this->_step_number = 0;
        }
      }
      else
      {
        if (this->_step_number == 0)
        {
          this->_step_number = this->_number_of_steps;
        }
        this->_step_number--;
      }
      // decrement the steps left:
      steps_left--;

      stepMotor(this->_step_number % 4);
    }
  }
}

void Stepper::stepMotor(int thisStep)
{

  switch (thisStep)
  {
  case 0: // 1010
    _motor_pin_1.write(1);
    _motor_pin_2.write(0);
    _motor_pin_3.write(1);
    _motor_pin_4.write(0);
    break;
  case 1: // 0110
    _motor_pin_1.write(0);
    _motor_pin_2.write(1);
    _motor_pin_3.write(1);
    _motor_pin_4.write(0);  
    break;
  case 2: // 0101
    _motor_pin_1.write(0);
    _motor_pin_2.write(1);
    _motor_pin_3.write(0);
    _motor_pin_4.write(1);  
    break;
  case 3: // 1001
    _motor_pin_1.write(1);
    _motor_pin_2.write(0);
    _motor_pin_3.write(0);
    _motor_pin_4.write(1);
    break;
  }
}
