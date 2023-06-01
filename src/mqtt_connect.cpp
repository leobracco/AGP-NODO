#include "Arduino.h"
#include "config.h"
#include "mqtt_connect.h"

unsigned long hash(const char *str)
{
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
  {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

void callback(char *topic, byte *payload, unsigned int length)
{

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);
  Serial.println("Inresa al callback");

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  if (strcmp(topic, "/AGP-UDP/SECCIONES") == 0)
  {
    int secciones[64];

    for (int i = 0; i < 64; i++)
    {

      secciones[i] = doc[String(i + 1)];
      Serial.print("Seccion ");
      Serial.print((i + 1));
      Serial.print(":");
      Serial.println(secciones[i]);
    }
  }
  if (strcmp(topic, "/AGP-UDP/VELOCIDAD") == 0)
  {
    float angulo = doc["angulo_giro"];
    speedKmH = doc["velocidad"];
    Serial.print("Velocidad: ");
    Serial.println((speedKmH));
    Serial.print("Angulo de giro");
    Serial.println(angulo);
  }
  char topicCal[50];
  sprintf(topicCal, "/AGP-APP/NODO/%d/CALIBRACION", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicCal) == 0)
  {

    if (saveConfig(doc, "/CalConfig.json"))
    {
      Serial.print("CAl");
    }
  }
  char topicReset[50];
  sprintf(topicReset, "/AGP-APP/NODO/%d/RESET", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicReset) == 0)
  {

    
    
    delay(2000);
    ESP.restart(); 
  }
  char topicPID[50];
  sprintf(topicPID, "/AGP-APP/NODO/%d/PID", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicPID) == 0)
  {

    if (saveConfig(doc, "/PidConfig.json"))
    {
      Serial.print("Config PID");
      Pid.FlowEnabled = doc["FlowEnabled"];
      Pid.RateError = doc["RateError"];
      Pid.Debounce = doc["Debounce"];
      Pid.KP = doc["KP"];
      Pid.KI = doc["KI"];
      Pid.KD = doc["KD"];
      Pid.Deadband = doc["Deadband"];
      Pid.BrakePoint = doc["BrakePoint"];
      Pid.UseMultiPulses = doc["UseMultiPulses"];
      Pid.FlowOnDirection = doc["FlowOnDirection"];
      Pid.FlowPin = doc["FlowPin"];
    }
  }
}

void connectToMQTT(WiFiClient &espClient, PubSubClient &client)
{
  client.setClient(espClient);
  client.setServer(MQTTConf.BrokerAddress, atoi(MQTTConf.Port));
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.print("Conectando a MQTT Broker...");
    if (client.connect(MQTTConf.ClientID))
    {
      Serial.println("Conexión exitosa");
      Serial.print("Se suscribe a:");

      client.subscribe("/AGP-UDP/#");

      Serial.print("NODO ID");
      Serial.println(Nodo.IdNodo);

      char topic[50];
      sprintf(topic, "/AGP-APP/NODO/%d/#", std::stoi(Nodo.IdNodo));
      const char *topicChar = topic;
      client.subscribe(topicChar);
    }
    else
    {
      Serial.print("Falló la conexión con el error ");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}