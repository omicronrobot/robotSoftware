# WifiClient

This is the omciron wificlient that connect to the wifi for the mobile platform main mcu. The wificlient is used to connect to the backend message broker to receive data from the handmotion controller and the mobile applications. It also sends the message to the main mcu via UART communication protocol. The message contains:

1. Pitch - Used to move in the forward and backward direction.
2. Roll - Used to move in the left or right direction.
3. Yaw - Used to turn the robot perpendicular to the surface.

The WifiClient should be able to:

1. Get sensor data in form of roll, yaw and pitch from HandMotion Client and Mobile Application.
2. Send sensor data via UART to the mainmcu.

Additionally we want the wificlient to be able to

1. Send data back to the clients for bidirectional control

## Setup

1. Install PlatformIO - Follow [Official documentation](https://platformio.org/install)
2. Install esptools - Follow [Official documentation](https://github.com/espressif/esptool)
3. Install Make - Follow [official documentation](https://www.gnu.org/software/make/)

## Write Code
