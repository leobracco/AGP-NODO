#include <Arduino.h>

#include <PubSubClient.h>

#include <agp_webserver.h>

#include "config.h"
#include "wifi_connect.h"
#include "mqtt_connect.h"
#include "rate.h"
#include "control.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <WiFiUdp.h>



WiFiClient espClient;
PubSubClient client(espClient);



PidConfig Pid;
CalConfig Cal;
MotorConfig Motor;
MQTTConfig MQTTConf;
WifiConfig WifiConf;
NodoConfig Nodo;
char mqtt_server[40] = "192.168.1.17";
char mqtt_port[6] = "1883";
char mqtt_user[20];
char mqtt_pass[20];



bool erase = false;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }
  delay(1000);
  if (erase)
  {

    delay(2000);
    ESP.restart();
  }

  strcpy(Nodo.Version, "1.0.0");
  Nodo.Version[sizeof(Nodo.Version) - 1] = '\0';
  uint32_t chipId = 0;
  bool res;
 
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);

  sprintf(MQTTConf.ClientID, "AGP-NODO-%d", ESP.getChipModel());
  

  delay(2000);
  ConfigureWifi();

  ConfigureMqtt();
  ConfigureNodo();

  ConfigPid();
  ConfigMotor();
  ConfigCalibracion();
  // ConfigNetWork();
  delay(1000);

  //if (connectToWiFi())
  if (enableAPWiFi())
  {
    Serial.print("Conectado a la red:");
    Serial.println(WifiConf.SSID);
    Serial.print("Con la IP:");
     Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("\nNo se pudo conectar al WiFi en 10 segundos.");
  }
  
  setupServer();
  //connectToMQTT(espClient, client);
  //setup_interrupciones();
}

void loop()
{
   server.handleClient();
  
  /*if (!client.connected())
  {
    connectToMQTT(espClient, client);
  }
  client.loop();
  loop_control();
  */
}