#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "mpu6050.h"
#include "defs.h"
#include "functions.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;
// MPU6050 mpu(0x69);

// We will be using yaw/pitch/roll angles (in degrees)
// calculated from the quaternions coming from the FIFO.
// Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)

// MPU control/status vars
bool dmpReady = false;	// set true if DMP init was successful
uint8_t devStatus;		// return status after each device operation (0 = success, !0 = error)
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;		 // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];		 // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

SensorReadings::SensorReadings(/* args */)
{
// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
#endif
	// initialize device
	debugln("Initializing I2C devices...");
	mpu.initialize();

	// verify connection
	debugln("Testing device connections...");
	debugln(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	// load and configure the DMP
	debugln("Initializing DMP...");
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
		debugln("Enabling DMP...");
		mpu.setDMPEnabled(true);

		// set our DMP Ready flag so the main function knows it's okay to use it
		dmpReady = true;
	}
	else
	{
		debug("DMP Initialization failed (code ");
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
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		sensorData.yaw = ypr[0] * 180 / M_PI;
		sensorData.pitch = ypr[1] * 180 / M_PI;
		sensorData.roll = ypr[2] * 180 / M_PI;
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

void SensorReadings::SendData() {
	SendMQTTData(sensorData);
}

float SensorReadings::getPitch() { return sensorData.pitch; }
float SensorReadings::getRoll() { return sensorData.roll; }
float SensorReadings::getYaw() { return sensorData.yaw; }
int SensorReadings::getState() { return sensorData.state; }
