#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <SoftwareSerial.h>
#include <SerialTransfer.h>
#include <ArduinoJson.h>
#include <string.h>

#define WLAN_SSID "wifi_name"
#define WLAN_PASSWORD "wifi_password"
#define MQTT_TOPIC "mqtt_topic"
#define MQTT_HOST "mqtt_host"
#define MQTT_PORT 1883
#define RX_PIN 1
#define TX_PIN 2
#define UART_BAUDRATE 9600
#define SERIAL_BAUDRATE 115200

#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

struct Data
{
  float pitch;    // The pitch for speed control of the robot. Speed is either max forward or zero or max revers. float -1 to +1.
  uint16_t yaw;   // Yaw used to control the direction of the robot. Rotation about perpendicular axis to the floor. Int 0 to 360.
  char *metadata; // The metadata of the message. It can be used to add any payload to the message for example the publisher.
};

WiFiEventHandler _wifi_connect_handler;
WiFiEventHandler _wifi_disconnect_handler;
AsyncMqttClient _mqtt_client;
Ticker _mqtt_reconnect_timer;
Ticker _wifi_reconnect_timer;
SerialTransfer _serial_transfer;
SoftwareSerial _esp_serial(RX_PIN, TX_PIN);
uint16_t _send_size = 0;

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

void SendData(char *string_data)
{
  Data data;
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, string_data);
  if (error)
  {
    return;
  }
  data.pitch = doc["pitch"];
  data.yaw = doc["yaw"];
  String pitch = String(data.pitch, 1);
  String yaw = String(data.yaw);
  String senddata = pitch + yaw;
  if (_esp_serial.available() > 0)
  {
    _serial_transfer.txObj(senddata, _send_size);
    _serial_transfer.sendData(_send_size);
  }
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  debugln("Publish received.");
  debug("  topic: ");
  debugln(topic);
  SendData(payload);
}

void setup()
{
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  _esp_serial.begin(UART_BAUDRATE);
  _serial_transfer.begin(_esp_serial);

  Serial.begin(SERIAL_BAUDRATE);

  _wifi_connect_handler = WiFi.onStationModeGotIP(_on_WiFi_connect);
  _wifi_disconnect_handler = WiFi.onStationModeDisconnected(_on_WiFi_disconnect);

  _mqtt_client.onConnect(_on_MQTT_connect);
  _mqtt_client.onDisconnect(onMqttDisconnect);
  _mqtt_client.onSubscribe(onMqttSubscribe);
  _mqtt_client.onUnsubscribe(onMqttUnsubscribe);
  _mqtt_client.onMessage(onMqttMessage);
  _mqtt_client.setServer(MQTT_HOST, MQTT_PORT);
  _connect_to_WiFi();
}

void loop()
{
}