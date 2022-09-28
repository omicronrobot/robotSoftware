# MainMCU

This is the omciron mobile platform main mcu. The mainmcu is used to drive the DC motors for translation motion based on poll and pitch. It also drives the stepper motors based on yaw for direction. The main MCU receives data from the WifiClient using UART protocol.

1. Pitch - Used to move in the forward and backward direction.
2. Roll - Used to move in the left or right direction.
3. Yaw - Used to turn the robot perpendicular to the surface.

The HandMotion should be able to:

1. Get sensor data in form of roll, yaw and pitch from WifiClient.
2. Drive DC motors for translational motion.
3. Drive Stepper motors for rotational motion.

Additionally we want the hand motion to be able to

1. Communicate back to the WiFiClient

## Setup

1. Install PlatformIO - Follow [Official documentation](https://platformio.org/install)
2. Install esptools - Follow [Official documentation](https://github.com/espressif/esptool)
3. Install Make - Follow [official documentation](https://www.gnu.org/software/make/)

## Write Code
