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
#include <led.h>
#include <boton.h>

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
bool isConnect = false;
Led WifiGreen(33);
Led WifiRed(32);
Led MqttGreen(22);
Led MqttRed(23);
const int buttonPin = 13;     // El número de pin del botón
const int pullupDelay = 5000; // Tiempo en milisegundos para detectar la presión prolongada

ButtonConfiguration buttonConfig = {buttonPin, pullupDelay};
ButtonPress buttonPress(buttonConfig);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  MqttRed.on();
  WifiRed.on();
  buttonPress.setup();
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

  if (connectToWiFi())
  {
    
    WifiRed.off();
    WifiGreen.on();
    Serial.print("Conectado a la red:");
    Serial.println(WifiConf.SSID);
    Serial.print("Con la IP:");
    Serial.println(WiFi.localIP());
    isConnect = true;
  }
  else
  {
    enableAPWiFi();
    
    Serial.println("\nNo se pudo conectar al WiFi en 10 segundos.");
  }

  if (!isConnect)
    setupServer();
  else
  {
    //connectToMQTT(espClient, client);
    setup_interrupciones();
  }
}

void loop()
{
  if (buttonPress.loop())
  {
    if (isConnect && client.connected())
    {
      Serial.println("Enviar info al broker");
      MqttRed.off();
      MqttGreen.off();
      delay(500);
      MqttGreen.update();
      MqttRed.update();
      delay(500);
      SendNodo(client);
    }
    else
    {
      Serial.println("Reset");
      ResetConfig();
    }
  }
  if (!isConnect)
  {
    server.handleClient();
    WifiGreen.update();
    MqttRed.on();
  }
  else
  {
    if (!client.connected())
    {
      MqttGreen.off();
      MqttRed.update();
      if (!buttonPress.loop())
        connectToMQTT(espClient, client);
    }
    else
    {
      MqttRed.off();
      WifiRed.off();
      MqttGreen.on();
      WifiGreen.on();

      client.loop();
      loop_control();
    }
  }
  
}
