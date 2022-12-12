#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const char *ssid = "0x6f736f646f-rodney-osodo";
const char *password = "Omicron1234";
const char *mqttServer = "10.42.0.1";
const int mqttPort = 1883;
const char *mqttUser = "cfc7a6c8-941e-4d72-be17-dbcc24b17fa3";
const char *mqttPassword = "7686eb82-d6b5-4513-b559-d867c8ed85e3";
const char *mqttTopic = "channels/fadfba2a-297c-49ef-87f3-c3b4745ce2ec/messages";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
SoftwareSerial mainmcuserial(18, 21);

void setup()
{
  Serial.begin(115200);
  mainmcuserial.begin(115200);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }
  Serial.println("Booting");
  connectToWiFi();
  setupOTA();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop()
{
  ArduinoOTA.handle();
  if (!client.connected())
  {
    connectToMQTT();
  }
  client.loop();
}

void connectToWiFi()
{
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  delay(2000);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(2000);
  }

  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT()
{
  Serial.println("Connecting to MQTT...");
  while (!client.connected())
  {
    if (client.connect("omicron-wificlient", mqttUser, mqttPassword))
    {
      Serial.println("Connected to MQTT.");
      client.subscribe(mqttTopic);
    }
    else
    {
      Serial.print("Failed to connect to MQTT. Error code: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setupOTA()
{
  ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname("omicron-wificlient");
  ArduinoOTA
      .onStart([]()
               {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String spayload = (char *)payload;
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 90;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonArray &root = jsonBuffer.parseArray(spayload);
  JsonObject &root_0 = root[0];
  JsonObject &root_1 = root[1];
  float pitch = root_0["v"];
  int yaw = root_1["v"];
  //  mainmcuserial.print("pitch:");
  mainmcuserial.print(pitch, 2);
  mainmcuserial.print(":");
  mainmcuserial.println(format_yaw(yaw));
  Serial.print("pitch:");
  Serial.print(pitch, 2);
  Serial.print("yaw:");
  Serial.println(format_yaw(yaw));
}

String format_yaw(int yaw)
{
  String zeros = "";
  if (yaw < 100)
    zeros += "0";
  if (yaw < 10)
    zeros += "0";
  zeros += yaw;
  return zeros;
}