#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "imu.h"
#include "defs.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

SensorReadings::SensorReadings(/* args */)
{
	bool dmpReady = false; // set true if DMP init was successful

// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
#endif
	// initialize device
	debugln("INITIALIZING I2C DEVICES...");
	mpu.initialize();

	// verify connection
	debugln("TESTING DEVICE CONNECTIONS...");
	debugln(mpu.testConnection() ? F("MPU6050 CONNECTION SUCCESSFUL") : F("MPU6050 CONNECTION FAILED"));

	// load and configure the DMP
	debugln("INITIALIZING DMP...");
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus == 0)
	{
		// Calibration Time: generate offsets and calibrate our MPU6050
		mpu.CalibrateAccel(6);
		mpu.CalibrateGyro(6);
		mpu.PrintActiveOffsets();
		// turn on the DMP, now that it's ready
		debugln("ENABLING DMP...");
		mpu.setDMPEnabled(true);

		// set our DMP Ready flag so the main function knows it's okay to use it
		dmpReady = true;
	}
	else
	{
		debug("DMP INITIALIZING FAILED (CODE ");
		debug(devStatus);
		debugln(")");
	}
}

SensorReadings::~SensorReadings()
{
	// TODO empty sensorData
}

void SensorReadings::Read(/* args */)
{
	// if programming failed, don't try to do anything
	if (!dmpReady)
		return;
	// read a packet from FIFO
	if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
	{ // Get the Latest packet
		// display Euler angles in degrees
		mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &quaternion);
		mpu.dmpGetYawPitchRoll(yawpitchroll, &quaternion, &gravity);
		sensorData.yaw = yawpitchroll[0] * 180 / M_PI;
		sensorData.pitch = yawpitchroll[1] * 180 / M_PI;
		sensorData.roll = yawpitchroll[2] * 180 / M_PI;
	}
}

void SensorReadings::PrintData()
{
	debug("READINGS yaw: ");
	debug(sensorData.yaw);
	debug(" pitch: ");
	debug(sensorData.pitch);
	debug(" pitch: ");
	debug(sensorData.pitch);
	debugln();
}

Data SensorReadings::GetData()
{
	return sensorData;
}

float SensorReadings::getPitch() { return sensorData.pitch; }
float SensorReadings::getRoll() { return sensorData.roll; }
float SensorReadings::getYaw() { return sensorData.yaw; }
int SensorReadings::getState() { return sensorData.state; }
