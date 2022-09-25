#ifndef MPU6050_H
#define MPU6050_H
#include "defs.h"


class SensorReadings
{
private:
    // This is the sensor Data from hand motion controller
    // It contains the pitch, roll, yaw and state values
    Data sensorData;

public:
    // SensorReadings This initializes the MPU6050 sensor onboard
    SensorReadings(/* args */);
    ~SensorReadings();

    // Read picth, roll and yaw values from the MPU6050 sensor
    void Read();

    // PrintData used for debugging purposes
    // It prints out the sensorData on the console
    void PrintData();

    // GetData gets the sensorData to be sent over wifi using mqtt protocol
    Data GetData();

    // getPitch gets the pitch of the handmotion sensor
    float getPitch();

    // getRoll gets the roll of the handmotion sensor
    float getRoll();

    // getYaw gets the yaw of the handmotion sensor
    float getYaw();

    // getState gets the state of the handmotion sensor
    int getState();
};

#endif