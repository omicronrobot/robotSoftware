#ifndef __DEFINITIONS_H_
#define __DEFINITIONS_H_


#define ESP_DRD_USE_SPIFFS true

#include <Arduino.h> 
#include <FS.h>
//#include <SPIFFS.h> 
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ESP_DoubleResetDetector.h>
#include <ArduinoJson.h>
#include "KalmanMPU6050.h"

  
#define JSON_CONFIG_FILE "/sample_config.json"
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0


#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x)
#endif


//output pins 
#define OnBoard_LED_PIN  2   // OnBoard_LEDLED PIN 
#define LED_PIN          2//13  // LED PIN  
#define TRIGGER_PIN      0   // Force reset push button pin

#define blinkRate_wm  150     // ms blink rate while waiting for wifimanager button wiFi change
#define blinkRate_ws  1000    // ms blink rate while working successfully  

#define Yaw_Offset    0.5    // yaw offset for calibration         
/* 
  ======= ==========    ====================================================
  VCC     VU (5V USB)   3.3V 
  GND     G             Ground
  SCL     D1 (GPIO05)   I2C clock
  SDA     D2 (GPIO04)   I2C data
  XDA     not connected
  XCL     not connected
  AD0     not connected
  INT     not connected Interrupt pin

*/

#endif /* __DEFINITIONS_H_ */