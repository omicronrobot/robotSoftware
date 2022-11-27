#include "mbed.h"
#include <Defs.h>
#include <Drive.h>
#include <Uart.h>

// DigitalOut onboard_led(PC_13);
// Motor 1 and 2 as labelled in PCB
StepperPins stepper_pins(PA_4, PA_5, PB_8, PB_9);
DigitalOut stepper_1_ena(PA_0);
DigitalOut stepper_1_enb(PA_1);
// Motor 5 as labelled in PCB
DCPins front_left(PA_6, PB_15, PB_14);
// Motor 6 as labelled in PCB
DCPins front_right(PA_7, PA_11, PA_12);
// Motor 7 as labelled in PCB
DCPins back_left(PB_0, PB_3, PA_8);
// Motor 8 as labelled in PCB
DCPins back_right(PB_1, PB_12, PB_13);
DrivePins drive_pins(front_left, front_right, back_left, back_right);

Drive drive(drive_pins, stepper_pins, 60);
Serial serial_mon(PA_9, PA_10, 9600);
// Serial serial_mon(USB_DM, USB_DP, 9600);
// Uart uart(USB_DM, USB_DP, 9600);

int main()
{
    serial_mon.printf("Program Start\n");
    stepper_1_ena = 1;
    stepper_1_enb = 1;
    drive.init(50, 0.1f);
    float count = 0.0;
    while (1)
    {
        // Data data = uart.ReceiveData();
        Data data;
        data.pitch = count;
        data.yaw = 90;
        serial_mon.printf("%f\n", data.pitch);
        drive.drive(data.pitch, data.yaw);
        // onboard_led = !onboard_led;
        wait(5);
        count = count + 0.1;
        if (count >= 1.0)
        {
            count = -1.0;
        }
    }
}

// #include "mbed.h"
// #include <DRV8870.h>

// Serial serial_mon(PA_9, PA_10,9600);
// DigitalOut _top_left_en(PA_0);
// Motor m1(PA_4, PA_5);

// int main()
// {
//     serial_mon.printf("Program Start\n");
//     _top_left_en = 1;
//     m1.setPeriod(0.1f);
//     while (1)
//     {
//         m1.setSpeed(1.0f);
//         wait(10);
//         m1.setSpeed(-1.0f);
//         wait(10);
//     }
// }

// #include "mbed.h"
// #include <Drive.h>

// StepperPins stepper_pins(PB_1, PB_2, PB_13, PB_12);
// DCPins front_left(PA_0, PA_4, PA_5);
// DCPins front_right(PA_1, PB_8, PB_9);
// DCPins back_left(PA_2, PB_6, PB_7);
// DCPins back_right(PA_3, PB_4, PB_5);
// DrivePins drive_pins(front_left, front_right, back_left, back_right);

// Drive drive(drive_pins, stepper_pins, 360, 100, 1);
// DigitalOut onboard_led(PC_13);
// Serial serial_mon(PA_9, PA_10, 9600);

// int main()
// {
//     serial_mon.printf("Program Start\n");
//     drive.init(50, 0.1f);
//     float counter = -1.0;
//     while (1)
//     {
//         onboard_led = !onboard_led;
//         drive.drive(counter, 90);
//         wait(1);
//         counter = counter + 0.1;
//         if (counter >= 1.0)
//         {
//             counter = -1.0;
//         }
//     }
// }

// #include "mbed.h"
// #include <SoftPWM.h>

// SoftPWM m1(PA_4);

// DigitalOut onboard_led(PC_13);
// Serial serial_mon(PA_9, PA_10, 9600);

// int main()
// {
//     serial_mon.printf("Program Start\n");
//     m1.period(0.1f);
//     m1.write(0.9f);
//     while (1)
//     {
//         serial_mon.printf("This is the bare metal blinky example running on Mbed OS\n");
//         onboard_led = !onboard_led;
//         wait(5);
//     }
// }

// #include "mbed.h"

// DigitalOut myled(PA_0);

// int main() {
//     while(1) {
//         myled = 1;
//         wait(0.2);
//         myled = 0;
//         wait(0.2);
//     }
// }