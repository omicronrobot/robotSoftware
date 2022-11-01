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
#define LED_PIN         16//2  //LED PIN INTERNAL
#define TRIGGER_PIN      0  //Force reset

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