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
void listAllFilesInDir(String dir_path)
{
	Dir dir = LittleFS.openDir(dir_path);
	while(dir.next()) {
		if (dir.isFile()) {
			// print file names
			Serial.print("File: ");
			Serial.println(dir_path + dir.fileName());
		}
		if (dir.isDirectory()) {
			// print directory names
			Serial.print("Dir: ");
			Serial.println(dir_path + dir.fileName() + "/");
			// recursive file listing inside new directory
			listAllFilesInDir(dir_path + dir.fileName() + "/");
		}
	}
}
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
  delay(1000);
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
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
