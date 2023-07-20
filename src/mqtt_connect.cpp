#include "Arduino.h"
#include "config.h"
#include "mqtt_connect.h"
#include "control.h"
#include "pid.h"

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
  

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  if (strcmp(topic, "/AGP-UDP/SECCIONES") == 0)
  {

    if (doc[String(Nodo.IdSection)] == 0)
      Nodo.Active = false;
    else
      Nodo.Active = true;

    speedKmH = doc["speedKmH"];
  }
  /*if (strcmp(topic, "/AGP-UDP/VELOCIDAD") == 0)
  {
    float angulo = doc["angulo_giro"];
    speedKmH = doc["velocidad"];
    if (DEBUG)
    {
      Serial.print("Velocidad: ");
      Serial.println((speedKmH));
      Serial.print("Angulo de giro");
      Serial.println(angulo);
    }
  }*/

  char topicMotor[50];
  sprintf(topicMotor, "/NODO/MOTOR/%d/MOTOR", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicMotor) == 0)
  {

    if (saveConfig(doc, "/MotorConfig.json"))
    {
      Motor.ControlType = doc["ControlType"];
      Motor.DirPin = doc["DirPin"];
      Motor.MaxPWM = doc["MaxPWM"];
      Motor.MinPWM = doc["MinPWM"];
      Motor.PWMPin = doc["PWMPin"];
      //pid.SetOutputLimits(Motor.MinPWM, Motor.MaxPWM);
    }
  }
  char topicCal[50];
  sprintf(topicCal, "/NODO/MOTOR/%d/CALIBRACION", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicCal) == 0)
  {

    if (saveConfig(doc, "/CalConfig.json"))
    {
      Cal.Auto = doc["Auto"];
      Cal.ManualPWM = doc["ManualPWM"];
      Cal.SetPoint = doc["SetPoint"];
      Cal.MinimumDose = doc["MinimumDose"];
      Cal.Working_Width = doc["Working_Width"];
      Cal.PulsePerRev = doc["PulsePerRev"];
      Cal.HolesPerPlate = doc["HolesPerPlate"];
      Cal.DosePerUnit = Cal.HolesPerPlate / Cal.PulsePerRev;
      Cal.SeedsPerMeter = doc["SeedsPerMeter"];
      Cal.SeedsPerHectare = doc["SeedsPerHectare"];
      Cal.KilosPerHectare = doc["KilosPerHectare"];
      Cal.LitersPerHectare = doc["LitersPerHectare"];
    }
  }
  char topicSample[50];
  sprintf(topicSample, "/NODO/MOTOR/%d/SampleDose", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicSample) == 0)
  {
      Cal.SampleDose=true;
      Cal.MaxTurns=doc["MaxTurns"];
      Cal.PWMMotor=doc["PWMMotor"];
      Cal.TotalPulses=0;
      

  }
  char topicLive[50];
  sprintf(topicLive, "/NODO/MOTOR/%d/LIVE", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicLive) == 0)
  {

    if (saveConfig(doc, "/PidConfig.json"))
    {
      Serial.print("Config PID");
      Pid.FlowEnabled = false;
      Pid.RateError = 0;
      Pid.Debounce = 0;
      Pid.KP = doc["KP"].as<double>();
      Pid.KI = doc["KI"].as<double>();
      Pid.KD = doc["KD"].as<double>();
      Pid.Deadband = doc["Deadband"];
      Pid.BrakePoint = 0;
      Pid.UseMultiPulses = true;
      Pid.FlowOnDirection = LOW;
      Pid.FlowPin = 21;
      Pid.pwmSetting = 0;
      //pid.SetTunings(Pid.KP, Pid.KI, Pid.KD);
      //pid.reset();
      serializeJson(doc, Serial);
    }
  }
  char topicTest[50];
  sprintf(topicTest, "/NODO/MOTOR/%d/TEST", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicTest) == 0)
  {

    Serial.println("Test");
  }
  char topicReset[50];
  sprintf(topicReset, "/NODO/MOTOR/%d/RESET", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicReset) == 0)
  {

    delay(2000);
    ESP.restart();
  }
  char topicPID[50];
  sprintf(topicPID, "/NODO/MOTOR/%d/PidConfig", std::stoi(Nodo.IdNodo));
  if (strcmp(topic, topicPID) == 0)
  {

    if (saveConfig(doc, "/PidConfig.json"))
    {
      Serial.print("Config PID");
      Pid.KP = doc["KP"].as<double>();
      Pid.KI = doc["KI"].as<double>();
      Pid.KD = doc["KD"].as<double>();
       Pid.Deadband = doc["Deadband"];
      Pid.FlowOnDirection = doc["FlowOnDirection"];
      Pid.FlowPin = doc["FlowPin"];
     
      //pid.SetTunings(Pid.KP, Pid.KI, Pid.KD);
      
      //printTuningParameters();
      
    }
  }
}

void connectToMQTT(WiFiClient &espClient, PubSubClient &client)
{
  client.setClient(espClient);
  client.setServer(MQTTConf.BrokerAddress, atoi(MQTTConf.Port));
  client.setCallback(callback);

  if (!client.connected())
  {
    Serial.print("Conectando a MQTT Broker...");
    if (client.connect(MQTTConf.ClientID))
    {
      Serial.println("Conexión exitosa");
      Serial.print("Se suscribe a:");

      // client.subscribe("/AGP-UDP/#");
      client.subscribe("/AGP-UDP/SECCIONES");

      Serial.print("NODO ID");
      Serial.println(Nodo.IdNodo);

      char topic[50];
      sprintf(topic, "/NODO/MOTOR/%d/#", std::stoi(Nodo.IdNodo));
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

void SendNodo(PubSubClient &client)
{

  // Convertir la estructura en un objeto JSON
  StaticJsonDocument<200> jsonDocument;
  jsonDocument["IdNodo"] = Nodo.IdNodo;
  jsonDocument["IdSection"] = Nodo.IdSection;
  jsonDocument["IdSeed"] = Nodo.IdSeed;
  jsonDocument["Version"] = Nodo.Version;
  jsonDocument["SeedPin"] = Nodo.SeedPin;

  // Convertir el objeto JSON a una cadena JSON
  char jsonString[200];
  serializeJson(jsonDocument, jsonString);
  char topic[50];
  sprintf(topic, "/NODO/%d/CONFIG", std::stoi(Nodo.IdNodo));
  const char *topicChar = topic;
  // Publicar el mensaje MQTT
  client.publish(topicChar, jsonString);
}

void SendMotor(PubSubClient &client)
{

  StaticJsonDocument<768> json;

  json["ControlType"] = Motor.ControlType;
  json["DirPin"] = Motor.DirPin;
  json["MaxPWM"] = Motor.MaxPWM;
  json["MinPWM"] = Motor.MinPWM;
  json["PWMPin"] = Motor.PWMPin;
  // Convertir el objeto JSON a una cadena JSON
  char jsonString[200];
  serializeJson(json, jsonString);
  char topic[50];
  sprintf(topic, "/NODO/%d/MOTOR", std::stoi(Nodo.IdNodo));
  const char *topicChar = topic;
  // Publicar el mensaje MQTT
  client.publish(topicChar, jsonString);
}

void SendMotorStatus(PubSubClient &client)
{

  StaticJsonDocument<1024> json;

  json["RPM"] = PPM/600;
  json["PPM"] = PPM;
  json["PWM"] = Cal.PWMMotor;
  json["SetPoint"] = Cal.SetPoint;
  json["SeedsPerMeter"] = Cal.SeedsPerMeter;
  json["KilosPerHectare"] = Cal.KilosPerHectare;
  json["LitersPerHectare"] = Cal.LitersPerHectare;
  json["SeedsPerHectare"] = Cal.SeedsPerHectare;
  json["TotalPulses"] = Cal.TotalPulses;
  json["Error"] = Pid.RateError;
 
  json["PWMMotor"]=Cal.PWMMotor;
   
  char jsonString[256];
  serializeJson(json, jsonString);
  char topic[50];
  sprintf(topic, "/NODO/%d/MOTOR/STATUS", std::stoi(Nodo.IdNodo));
  const char *topicChar = topic;
  // Publicar el mensaje MQTT
  client.publish(topicChar, jsonString);
  // printTuningParameters();
}