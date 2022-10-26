#include "mbed.h"
#include <Defs.h>
#include <Drive.h>
#include <Uart.h>

StepperPins stepper_pins(PA_0, PA_1, PA_2, PA_3);
DCPins front_left(PA_0, PA_1, PA_2);
DCPins front_right(PA_0, PA_1, PA_2);
DCPins back_left(PA_0, PA_1, PA_2);
DCPins back_right(PA_0, PA_1, PA_2);
DrivePins drive_pins(front_left, front_right, back_left, back_right);

Drive drive(drive_pins, stepper_pins, 360, 100, 1);
Uart uart(PA_0, PA_1, 9600);

int main()
{
    drive.init(50, 0.01);
    while (1)
    {
        Data data = uart.ReceiveData();
        drive.drive(data.pitch, data.yaw);
        ThisThread::sleep_for(1000);
    }
}
