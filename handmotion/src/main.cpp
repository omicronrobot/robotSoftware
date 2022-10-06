#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define WLAN_SSID "wifi_name"
#define WLAN_PASSWORD "wifi_password"
#define MQTT_TOPIC "mqtt_topic"
#define MQTT_HOST "mqtt_host"
#define MQTT_PORT 1883
#define SERIAL_BAUDRATE 115200
#define M_PI 3.14159265358979323846

#define DEBUG 2
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

WiFiEventHandler _wifi_connect_handler;
WiFiEventHandler _wifi_disconnect_handler;
AsyncMqttClient _mqtt_client;
Ticker _mqtt_reconnect_timer;
Ticker _wifi_reconnect_timer;
MPU6050 _mpu;
// MPU control/status vars
bool _dmp_ready;          // set true if DMP init was successful
uint8_t _dev_status;      // return status after each device operation (0 = success, !0 = error)
uint8_t _fifo_buffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion _quaternion;   // [w, x, y, z]         quaternion container
VectorFloat _gravity;     // [x, y, z]            gravity vector
float _yaw_pitch_roll[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

struct Data
{
  float pitch; // Rotation around the side-to-side axis
  float roll;  // Rotation around the front-to-back axis
  float yaw;   // Rotation around the vertical axis
};

void _connect_to_WiFi()
{
  debugln("Connecting to Wi-Fi...");
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
}

void _connect_to_MQTT()
{
  debugln("Connecting to MQTT...");
  _mqtt_client.connect();
}

void _on_WiFi_connect(const WiFiEventStationModeGotIP &event)
{
  debugln("Connected to Wi-Fi.");
  _connect_to_MQTT();
}

void _on_WiFi_disconnect(const WiFiEventStationModeDisconnected &event)
{
  debugln("Disconnected from Wi-Fi.");
  _mqtt_reconnect_timer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  _wifi_reconnect_timer.once(2, _connect_to_WiFi);
}

void _on_MQTT_connect(bool sessionPresent)
{
  debugln("Connected to MQTT.");
  debug("Session present: ");
  debugln(sessionPresent);
  uint16_t packetIdSub = _mqtt_client.subscribe(MQTT_TOPIC, 2);
  debug("Subscribing at QoS 2, packetId: ");
  debugln(packetIdSub);
  uint16_t packetIdPub = _mqtt_client.publish(MQTT_TOPIC, 2, true, "test 3");
  debug("Publishing at QoS 2, packetId: ");
  debugln(packetIdPub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  debugf("Disconnected from MQTT: %u.\n", static_cast<uint8_t>(reason));

  if (WiFi.isConnected())
  {
    _mqtt_reconnect_timer.once(2, _connect_to_MQTT);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  debugln("Subscribe acknowledged.");
  debug("  packetId: ");
  debugln(packetId);
}

void onMqttUnsubscribe(uint16_t packetId)
{
  debugln("Unsubscribe acknowledged.");
  debug("  packetId: ");
  debugln(packetId);
}

void SendData(Data data)
{
  String output;
  StaticJsonDocument<96> doc;

  doc["pitch"] = data.pitch;
  doc["roll"] = data.roll;
  doc["yaw"] = data.yaw;

  serializeJson(doc, output);

  uint16_t packetIdPub2 = _mqtt_client.publish(MQTT_TOPIC, 2, true, output.c_str());
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetIdPub2);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  debugln("Publish received.");
  debug("  topic: ");
  debugln(topic);
}

void onMqttPublish(uint16_t packetId)
{
  debugln("Publish acknowledged.");
  debug("  packetId: ");
  debugln(packetId);
}

void imuInit()
{
  {
    bool _dmp_ready = false; // set true if DMP init was successful

// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    // initialize device
    debugln("INITIALIZING I2C DEVICES...");
    _mpu.initialize();

    // verify connection
    debugln("TESTING DEVICE CONNECTIONS...");
    debugln(_mpu.testConnection() ? F("MPU6050 CONNECTION SUCCESSFUL") : F("MPU6050 CONNECTION FAILED"));

    // load and configure the DMP
    debugln("INITIALIZING DMP...");
    _dev_status = _mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    _mpu.setXGyroOffset(220);
    _mpu.setYGyroOffset(76);
    _mpu.setZGyroOffset(-85);
    _mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (_dev_status == 0)
    {
      // Calibration Time: generate offsets and calibrate our MPU6050
      _mpu.CalibrateAccel(6);
      _mpu.CalibrateGyro(6);
      _mpu.PrintActiveOffsets();
      // turn on the DMP, now that it's ready
      debugln("ENABLING DMP...");
      _mpu.setDMPEnabled(true);

      // set our DMP Ready flag so the main function knows it's okay to use it
      _dmp_ready = true;
    }
    else
    {
      debug("DMP INITIALIZING FAILED (CODE ");
      debug(_dev_status);
      debugln(")");
    }
  }
}

Data GetData()
{
  Data _sensor_data = {};
  // if programming failed, don't try to do anything
  if (!_dmp_ready)
    return _sensor_data;
  // read a packet from FIFO
  if (_mpu.dmpGetCurrentFIFOPacket(_fifo_buffer))
  { // Get the Latest packet
    // display Euler angles in degrees
    _mpu.dmpGetQuaternion(&_quaternion, _fifo_buffer);
    _mpu.dmpGetGravity(&_gravity, &_quaternion);
    _mpu.dmpGetYawPitchRoll(_yaw_pitch_roll, &_quaternion, &_gravity);
    _sensor_data.yaw = _yaw_pitch_roll[0] * 180 / M_PI;
    _sensor_data.pitch = _yaw_pitch_roll[1] * 180 / M_PI;
    _sensor_data.roll = _yaw_pitch_roll[2] * 180 / M_PI;
  }
  return _sensor_data;
}
void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  imuInit();
  _wifi_connect_handler = WiFi.onStationModeGotIP(_on_WiFi_connect);
  _wifi_disconnect_handler = WiFi.onStationModeDisconnected(_on_WiFi_disconnect);

  _mqtt_client.onConnect(_on_MQTT_connect);
  _mqtt_client.onDisconnect(onMqttDisconnect);
  _mqtt_client.onSubscribe(onMqttSubscribe);
  _mqtt_client.onUnsubscribe(onMqttUnsubscribe);
  _mqtt_client.onPublish(onMqttPublish);
  _mqtt_client.onMessage(onMqttMessage);
  _mqtt_client.setServer(MQTT_HOST, MQTT_PORT);
  _connect_to_WiFi();
}

void loop()
{
  Data data = GetData();
  SendData(data);
  delay(100);
}
