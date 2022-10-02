#include "DRV8870.h"

Motor::Motor(PinName pwm1, PinName pwm2) : _pwm1(pwm1), _pwm2(pwm2)
{
    // Set initial condition of PWM
    _pwm1.period(0.001);
    _pwm1.write(0.0f);
    _pwm2.period(0.001);
    _pwm2.write(0.0f);
}

void Motor::speed(float speed)
{
    if (speed > 0.0)
    {

        _pwm1.write(abs(speed));
        _pwm2.write(0.0f);
    }
    else
    {
        _pwm1.write(0.0f);
        _pwm2.write(abs(speed));
    }
}
void Motor::period(float period)
{

    _pwm1.period(period);
    _pwm2.period(period);
}

void Motor::brake(int mode)
{

    if (mode == COAST)
    {
        _pwm1.write(0.0f);
        _pwm2.write(0.0f);
    }
    else if (mode == BRAKE)
    {
        _pwm1.write(1.0f);
        _pwm2.write(1.0f);
    }
}
