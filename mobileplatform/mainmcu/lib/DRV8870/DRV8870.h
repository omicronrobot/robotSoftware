/** DRV8870
 * @section DESCRIPTION
 *
 * mbed simple DRV8833 H-bridge motor controller
 *
 *
 *    x_PWM1 x_PWM2    Mode
 *      0      0       Coast/Fast decay
 *      0      1       Reverse
 *      1      0       Forward
 *      1      1       Brake/slow decay
 */

#ifndef MOBILEPLATFORM_MAINMCU_LIB_DRV8870_DRV8870_H_
#define MOBILEPLATFORM_MAINMCU_LIB_DRV8870_DRV8870_H_

#include "mbed.h"
#include <SoftPWM.h>

#define COAST 1
#define BRAKE 0

class Motor
{
private:
    SoftPWM _pwm1;
    SoftPWM _pwm2;

public:
    /** Creates a DRV8870(H-bridge motor controller) control interface
     *
     * @param pwm1 A SoftPWM pin, tied to the IN1 Logic input and controls state of OUT1
     * @param pwm2 A SoftPWM pin, tied to the IN2 Logic input and controls state of OUT2
     *
     */
    Motor(PinName pwm1, PinName pwm2);

    /** Set the speed of the motor
     *
     * @param speed The speed of the motor as a normalised value between -1.0 and 1.0
     */
    void speed(float speed);

    /** Set the period of the pwm duty cycle.
     *
     * Wrapper for SoftPWM::period()
     *
     * @param seconds - Pwm duty cycle in seconds.
     */
    void period(float period);

    /** Brake the H-bridge coast or brake.
     *
     * Defaults to coast.
     * @param mode - Braking mode.COAST(default)or BRAKE.
     *
     */
    void brake(int mode = COAST);
};

#endif // MOBILEPLATFORM_MAINMCU_LIB_DRV8870_DRV8870_H_