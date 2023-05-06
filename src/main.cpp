#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "wifi_connect.h"
#include "mqtt_connect.h"
#include "config.h"
#include "rate.h"
#include "control.h"
#include <LittleFS.h>
#include <ArduinoJson.h>


WiFiClient espClient;
PubSubClient client(espClient);
SensorConfig Sensor;
void setup() {
  Serial.begin(115200);
  delay(1000);
  iniciarLittleFS();
  
  ConfigPid( );
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  connectToMQTT(espClient, client, MQTT_SERVER, MQTT_USERNAME, MQTT_PASSWORD);
  setup_interrupciones();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT(espClient, client, MQTT_SERVER, MQTT_USERNAME, MQTT_PASSWORD);
  }
  client.loop();
  loop_control();
  
}
