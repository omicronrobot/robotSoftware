#include "DRV8870.h"

Motor::Motor(PinName pwm1, PinName pwm2) : _pwm1(pwm1), _pwm2(pwm2)
{
    // Set initial condition of PWM
    this->_pwm1.period(0.1f);
    this->_pwm1.write(0.0f);
    this->_pwm2.period(0.1f);
    this->_pwm2.write(0.0f);
}

void Motor::setSpeed(float speed)
{
    if (speed > 0.0)
    {

        this->_pwm1.write(abs(speed));
        this->_pwm2.write(0.0f);
    }
    else
    {
        this->_pwm1.write(0.0f);
        this->_pwm2.write(abs(speed));
    }
}
void Motor::setPeriod(float period)
{

    this->_pwm1.period(period);
    this->_pwm2.period(period);
}

void Motor::brake(int mode)
{

    if (mode == COAST)
    {
        this->_pwm1.write(0.0f);
        this->_pwm2.write(0.0f);
    }
    else if (mode == BRAKE)
    {
        this->_pwm1.write(1.0f);
        this->_pwm2.write(1.0f);
    }
}
