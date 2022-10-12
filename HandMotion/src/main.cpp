
#define DEBUG 1        //set 1 for serial read statements

#include <definitions.h>
//All definitions and libraries used have been added to the definitions.h file under include

void (* resetFunc)(void) = 0;             // creating pointer at memory address 0

//RTC_DATA_ATTR unsigned int MQTT_conf = 0; //CHECK FOR CONFIG DUE TO MQTT SERVER OFF

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


DoubleResetDetector *drd;
bool shouldSaveConfig = false;


//MQTT
char MQTTIP[50] = "pi.local";
int MQTT_PORT = 1883;
bool EnableDebug = true;


WiFiManager wm;

void saveConfigFile()
{
  debugln(F("Saving config"));
  StaticJsonDocument<512> json;
  json["MQTTIP"] = MQTTIP;
  json["MQTT_PORT"] = MQTT_PORT;
  json["EnableDebug"] = EnableDebug;

  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
  if (!configFile)
  {
    debugln(F("failed to open config file for writing"));
  }

  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0)
  {
    debugln(F("Failed to write to file"));
  }
  configFile.close();
}

bool loadConfigFile()
{
  //clean FS, for testing
  // SPIFFS.format();

  //read configuration from FS json
  debugln(F("mounting FS..."));

   bool success = SPIFFS.begin(); 
  // if (SPIFFS.begin(false) || SPIFFS.begin(true))
  if (success) 
  {
    debugln(F("mounted file system"));
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      //file exists, reading and loading
      debugln(F("reading config file"));
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile)
      {
        debugln(F("opened config file"));
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);
        if (!error)
        {
          debugln(F("\nparsed json"));

          strcpy(MQTTIP, json["MQTTIP"]);
          MQTT_PORT = json["MQTT_PORT"].as<int>();
          EnableDebug = json["EnableDebug"].as<bool>();

          return true;
        }
        else
        {
          debugln(F("failed to load json config"));
        }
      }
    }
  }
  else
  {
    debugln(F("failed to mount FS"));
  }


  //end read
  return false;
}
//callback notifying us of the need to save config
void saveConfigCallback()
{
  debugln(F("Should save config"));
  shouldSaveConfig = true;
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  debugln(F("Entered Conf Mode"));

  debug("Config SSID: ");
  debugln(myWiFiManager->getConfigPortalSSID());

  debug("Config IP Address: ");
  debugln(WiFi.softAPIP());
}

void LED(bool state)
{
  digitalWrite(LED_PIN, !state);
}

bool force_state = false;
void blink_para(int timeout_){
  
  unsigned int previous_time = millis();
  unsigned int current_time = millis();

  bool Lstate = true;

  while((current_time - previous_time) < timeout_){
      
    volatile unsigned int previous_t;
    unsigned int current_t = millis();
    int t = 200;
    
    if((current_t - previous_t) > t){
      LED(Lstate);
      Lstate = !Lstate;  
      previous_t = current_t; 
    }
    
    if(digitalRead(TRIGGER_PIN) == LOW) {
     force_state = true;
     
     debugln(F("button  pressed"));
     LED(true);
     break;
    }    
    current_time = millis();
  }
  LED(true);
}
 

bool forceConfig = false;
void waitForceConfig(){
  
  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  if (drd->detectDoubleReset())
  {
    debugln(F("Forcing config mode as there was a Double reset detected -- currently disabled"));
   // forceConfig = true;  // uncomment to enablr double reset funct 
  }

  bool spiffsSetup = loadConfigFile();
  if (!spiffsSetup)
  {
    debugln(F("Forcing config mode as there is no saved config"));
    forceConfig = true;
  }
   
  debugln(F("start b check"));
  blink_para(3000);
  force_state == 1 ? forceConfig = true : forceConfig = false;
  debugln(F("end of b check")); 

  //if(MQTT_conf == 1){
  //  MQTT_conf = 0;
  //  forceConfig = true;
  //  debugln(F("Forcing config mode to change MQTT Parameters -- could not connect to MQTT server")); 
  //}

   //WiFi.disconnect();
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  delay(10); 

}

void WifiManagersetup(){ 

  // wm.resetSettings(); // wipe settings

  //WiFiManager wm;

  //wm.resetSettings(); // wipe settings
  //set config save notify callback
  wm.setSaveConfigCallback(saveConfigCallback);
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);
  
  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
  // wm.setShowStaticFields(true); // force show static ip fields
  // wm.setShowDnsFields(true);    // force show dns field always
   
  // wm.setConnectTimeout(20); // how long to try to connect for before continuing
  wm.setConfigPortalTimeout(60); // auto close configportal after n seconds
  // wm.setCaptivePortalEnable(false); // disable captive portal redirection
  // wm.setAPClientCheck(true); // avoid timeout if client connected to softap

  // wifi scan settings
  // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
  // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
  // wm.setShowInfoErase(false);      // do not show erase button on info page
  // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons
  
  // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails


  //--- additional Configs params ---

  // Text box for IP adress
  WiFiManagerParameter custom_text_box("key_text", "Enter MQTT server IP Address", MQTTIP, 50); // 50 == max length

  // Text box for port
  char convertedValue[6];
  sprintf(convertedValue, "%d", MQTT_PORT); // Need to convert to string to display a default value.

  WiFiManagerParameter custom_text_box_num("key_num", "Enter MQTT server port", convertedValue, 7); // 7 == max length

  //Check Box
  char *customHtml;
  if (EnableDebug)
  {
    customHtml = "type=\"checkbox\" checked";
  }
  else
  {
    customHtml = "type=\"checkbox\"";
  }
  WiFiManagerParameter custom_checkbox("key_bool", "Enable Debug", "T", 2, customHtml); 

  wm.addParameter(&custom_text_box);
  wm.addParameter(&custom_text_box_num);
  wm.addParameter(&custom_checkbox);

  debugln(F("hello"));

  if (forceConfig)
  {
    if (!wm.startConfigPortal("HM_Phanthom", "clockX20"))
    {
      debugln(F("failed to connect and hit timeout"));
      delay(300);
       
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }
  else
  {
    if (!wm.autoConnect("HM_Phanthom", "clockX20"))
    {
      debugln(F("failed to connect and hit timeout"));
      delay(3000);
      // if we still have not connected restart and try all over again 
      ESP.restart();
      
      delay(5000);
    }
  }

  // If we get here, we are connected to the WiFi
  
  debugln(F(""));
  debugln(F("WiFi connected"));
  debugln(F("IP address: "));
  debugln(WiFi.localIP());

  // dealing with the user config values

  // Copy the IP value
  strncpy(MQTTIP, custom_text_box.getValue(), sizeof(MQTTIP));
  debug(F("MQTTIP: "));
  debugln(MQTTIP);

  //Convert the PORT value
  MQTT_PORT = atoi(custom_text_box_num.getValue());
  debug(F("MQTT_PORT: "));
  debugln(MQTT_PORT);

  //Handle the bool value
  EnableDebug = (strncmp(custom_checkbox.getValue(), "T", 1) == 0);
  debug(F("EnableDebug: "));
  if (EnableDebug)
  {
    debugln(F("Debug true"));
  }
  else
  {
    debugln(F("Debug false"));
  }

  //save the custom parameters to FS
  if (shouldSaveConfig)
  {
    saveConfigFile();
  }
}
 

void callback(char* topic, byte* payload, unsigned int length) {
  debug("Message arrived [");
  debug(topic);
  debug("] ");

  String payload_str = "";
  for (int i = 0; i < length; i++) {
    debug((char)payload[i]);
    payload_str += (char)payload[i];
  }
  
  debugln(F("Processing :"));
  debugln(payload_str);
  //e.g Message arrived [inTopic] 1

  for(int j =0; j < (payload_str.length() + 1); j++ ){
    
    if (payload_str[j] == '1') {
          
        debugln(F("1 Received"));
      
    }


  }

}

void force_config(){

      LED(true);          
      // start portal with 200 seconds runtime
      debugln(F("Restarting to Start config portal -- MQTT server connect fail"));
      delay(1000);
      //forceConfig = true;
      //WifiManagersetup();

      LED(false);
      //MQTT_conf = 1;
      
      //esp_deep_sleep(1000000);//test time  -- 1 secs(uS)
      
      delay(1000);
      
      // wm.setConfigPortalTimeout(200);
      
      // if (!wm.startConfigPortal("HM_Phanthom", "clockX20")) {
      //   debugln(F("failed to connect or hit timeout.\n Restarting..."));
      //   delay(300);
      //   LED(false);
      //   ESP.restart();
      // } else {
      //   //if you get here you have connected to the WiFi
      //   debugln(F("connected...yeey :)"));
      //   LED(false);
      // }
}

void reconnect() {
  int fail_count = 0; 
  // Loop until we're reconnected
  while (!client.connected()) {
    debug(F("Attempting MQTT connection..."));
    // Create a random client ID
    String clientId = "HANDMOTIONCLIENT-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      debugln(F("connected"));
      // Once connected, publish an announcement...
      client.publish("outTopic", "Program Started ..");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      if(fail_count == 10) {
        fail_count = 0;
        debugln(F("failed 10 times, starting config portal"));
        force_config();
      }
      
      fail_count = fail_count + 1;
      debug(F("failed, rc="));
      debug(client.state());
      debugln(F(" trying again in 2 seconds"));
      // 2 seconds before retrying
      delay(2000);
    }
  }
}

void upload(int16_t yaw, int16_t pitch, int16_t roll, int16_t raw_AX, int16_t raw_AY, int16_t raw_AZ, int16_t raw_GX, int16_t raw_GY, int16_t raw_GZ, uint32_t lrt)
{  
  //prints data to be sent
  //upload code
  debugln(F("\nData ____"));
  String senddata = "Y: " +(String)yaw + " P:" + (String)pitch+ " R:" + (String)roll;
  
  if (EnableDebug)
  {
    debugln(F("Raw data sent to MQTT server: "));
    senddata +=  "  RAX:" + (String)raw_AX + " RAY:" + (String)raw_AY + " RAZ:" + (String)raw_AZ + "  RGX:" + (String)raw_GX + " RGY:" + (String)raw_GY + " RGZ:" + (String)raw_GZ + " LastTIME:" + (String)lrt;
  }

  // debugln("***\n" + senddata + "\n***");

   if (!client.connected()) {
    reconnect();
  }
      // debug("\t\tAttempting MQTT connection...");
    // Create a random client ID
    String clientId = "HANDMOTIONCLIENT-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      // debugln(F("connected"));

    debug(F("\n outTopic: "));
    debugln(senddata);
    
     char mqttdata[100];
     senddata.toCharArray(mqttdata, 100);

    client.publish("outTopic", mqttdata);
    delay(150);
    }
 
}


void check_state(){
   
 
  String Cstate;
  //      if                         else 
  EnableDebug == 1 ? Cstate = " Open":Cstate = " Closed";
       
   
}
 

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold
    delay(500);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      debugln(F("Button Pressed"));
      
      LED(true);
                  
      // start portal w delay
      debugln(F("Starting config portal"));
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("HM_Phanthom", "clockX20")) {
        debugln(F("failed to connect or hit timeout"));
        delay(3000);
        //ESP.restart();
        LED(false);
      } else {
        //if you get here you have connected to the WiFi
        debugln(F("connected...yeey :)"));
        LED(false);
      }
    }
  }
}

void setup() { 

  if(DEBUG == 1)
  {
  Serial.begin(115200);
  }

  delay(100);
 
  
  debugln(F("\n\nStarting program"));
  
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, !LOW);
   
  LED(true);
  waitForceConfig();
  WifiManagersetup();
   
  //setup_wifi();
  randomSeed(micros());  
 
  client.setServer(MQTTIP, MQTT_PORT);   
  client.setCallback(callback);
   
 
  debugln(F("\n---------------------------------------------\n"));
  
  IMU::init();
  IMU::read();

  digitalWrite(LED_PIN, !LOW);
  pinMode(TRIGGER_PIN, INPUT);
  debugln(F("\n\t\tInit...\n"));
  
}

void loop() {

  check_state();
  if (!client.connected()) 
  {
      if ((WiFi.status() != WL_CONNECTED))
      {
       //resetFunc();
       ESP.restart();
      }
    reconnect();
  }


  IMU::read();
  // Serial.print(IMU::getRoll());
  // Serial.print(",");
  // Serial.println(IMU::getPitch());
  debug(IMU::getRoll());
  debug(F(","));
  debugln(IMU::getPitch());
  /// @brief ///////
  int yaw = 0;  //not implemented yet
  upload(yaw, IMU::getPitch(), IMU::getRoll(), IMU::getRawAccelX(),  IMU::getRawAccelY(),  IMU::getRawAccelZ(), IMU::getRawGyroX(), IMU::getRawGyroY(), IMU::getRawGyroZ(), IMU::getLastReadTime());

 

  client.loop();
  //drd->loop();   //uncomment for doublereset for config page functionality
  checkButton();   //boot pin to force config page
}