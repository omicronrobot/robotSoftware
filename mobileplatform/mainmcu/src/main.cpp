#include "mbed.h"

PwmOut led(C13);

int main()
{
    // specify period first
    led.period(4.0f);      // 4 second period
    led.write(0.50f);      // 50% duty cycle, relative to period
    while(1);
}

// PWM Docs - https://os.mbed.com/handbook/PwmOut