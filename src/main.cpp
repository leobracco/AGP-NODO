#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "wifi_connect.h"
#include "mqtt_connect.h"

#include "rate.h"
#include "control.h"
#include <LittleFS.h>
#include <ArduinoJson.h>


WiFiClient espClient;
PubSubClient client(espClient);
SensorConfig Sensor;
MQTTConfig MQTTConf;
WifiConfig WifiConf;
void setup() {
  Serial.begin(115200);
  delay(1000);
 
  if (LittleFS.begin()) {
        Serial.println(F("done."));
    } else {
        Serial.println(F("fail."));
    }
  delay(1000);
  ConfigPid( );
   ConfigNetWork();
  delay(1000);
  connectToWiFi();
  connectToMQTT(espClient, client );
  setup_interrupciones();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT(espClient, client);
  }
  client.loop();
  loop_control();
  
}
