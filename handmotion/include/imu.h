#ifndef IMU_H
#define IMU_H
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "defs.h"

// We will be using yaw/pitch/roll angles (in degrees)
// calculated from the quaternions coming from the FIFO.
// Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)

class SensorReadings
{
private:
    // This is the sensor Data from hand motion controller
    // It contains the pitch, roll, yaw and state values
    Data sensorData;
    MPU6050 mpu;

    // MPU control/status vars
    bool dmpReady;          // set true if DMP init was successful
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion quaternion; // [w, x, y, z]         quaternion container
    VectorFloat gravity;   // [x, y, z]            gravity vector
    float yawpitchroll[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

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