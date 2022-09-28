# HandMotion

This is the omciron handmotion client. The handmotion is used to track handmotion gestures from the hand and send them to the mobile platform to drive it. From the handmotion controller we get the roll, yaw and pitch data and sent it to the message broker.

1. Pitch - Used to move in the forward and backward direction.
2. Roll - Used to move in the left or right direction.
3. Yaw - Used to turn the robot perpendicular to the surface.

The HandMotion should be able to:

1. Get IMU sensor data in form of roll, yaw and pitch.
2. Connect to Wifi for communication purposes.
3. Connect to backend MQTT or HTTP broker.
4. Send data over MQTT to the message broker.

Additionally we want the hand motion to be able to

1. Filter sensor data from the IMU. kalman filter would be a great addition
2. Setup Wifi Manager to be able to change the WiFi credentials rather that building the software everytime
3. Setup Web Based Manager to input MQTT and WiFi details
4. Be able to subscribe to the message broker for start and stop recording

## Setup

1. Install PlatformIO - Follow [Official documentation](https://platformio.org/install)
2. Install esptools - Follow [Official documentation](https://github.com/espressif/esptool)
3. Install Make - Follow [official documentation](https://www.gnu.org/software/make/)

## Write Code
