#include <Arduino.h>

#include "config.h"

#include <ArduinoJson.h>

// Configuración de sensor
void iniciarLittleFS()
{
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }
}

String loadConfig(const char *FileName)
{
  String configContent = "";
  File configFile = LittleFS.open(FileName, "r");
  if (!configFile)
  {
    Serial.println("No se puede abrir el archivo en Load");
  }
  else
  {
    while (configFile.available())
    {
      configContent += char(configFile.read());
    }
    configFile.close();
  }
  return configContent;
}

bool saveConfig(DynamicJsonDocument &json,
                const char *FileName)
{

  File configFile = LittleFS.open(FileName, "w");
  if (!configFile)
  {
    Serial.println("No se puede abrir el archivo para grabar");
    return false;
  }
  else
    Serial.println("Se grabo correctamente");
  serializeJson(json, Serial);
  serializeJson(json, configFile);
  configFile.close();
  return true;
}
/**********************Carga las variables del PID desde el Archivo PidConfig.json*/
void ConfigPid()
{
  Serial.println("Ingresa a Config PID...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/PidConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Pid.BrakePoint = json["BrakePoint"];
  Pid.Deadband = json["Deadband"];
  Pid.Debounce = json["Debounce"];
  Pid.FlowEnabled = json["FlowEnabled"];
  Pid.FlowOnDirection = json["FlowOnDirection"];
  Pid.FlowPin = json["FlowPin"];
  Pid.KP = json["KP"].as<double>();
  Pid.KI = json["KI"].as<double>();
  Pid.KD = json["KD"].as<double>();
  Pid.pwmSetting = json["pwmSetting"];
  Pid.RateError = json["RateError"];
  Pid.UseMultiPulses = json["UseMultiPulses"];
  Pid.PulsePerRev = 600;
  Pid.IntegralUpperLimit = json["IntegralUpperLimit"];
  Pid.IntegralLowerLimit = json["IntegralLowerLimit"];
  Pid.DerivativeFilter = json["DerivativeFilter"];
}
/**********************Carga las variables de Calibracion desde el Archivo CalConfig.json*/
void ConfigCalibracion()
{
  Serial.println("Ingresa a Config Calibracon...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/CalConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  Cal.ManualAdjust = json["ManualAdjust"];
  Cal.MeterCal = json["MeterCal"];
  Cal.SetPoint = json["SetPoint"];
  Cal.TotalPulses = json["TotalPulses"];
  Cal.UPM = json["UPM"];
  Cal.Working_Width = json["Working_Width"];
  Cal.DosePerUnit = 20 / 600;
  Cal.dosagePerHectare = 80000;
}
/**********************Carga las variables del Motor desde el Archivo MotorConfig.json*/
void ConfigMotor()
{
  Serial.println("Ingresa a Config MOtor...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/MotorConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Motor.ControlType = json["ControlType"];
  Motor.DirPin = json["DirPin"];
  Motor.PWMPin = json["PWMPin"];
  Motor.MinPWM = json["MinPWM"];
  Motor.MaxPWM = json["MaxPWM"];
}
/**********************Carga las variables del Nodo desde el Archivo NodoConfig.json*/
void ConfigureNodo()
{
  Serial.println("Ingresa a Config Nodo...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/NodoConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  // Nodo.idnodo = json["idnodo"].as<char*>();
  strcpy(Nodo.IdNodo, json["IdNodo"]);
  strcpy(Nodo.IdSection, json["IdSection"]);
  strcpy(Nodo.IdSeed, json["IdSeed"]);
  strcpy(Nodo.Version, json["Version"]);
}
/**********************Carga las variables del Mqtt desde el Archivo MqttConfig.json*/
void ConfigureMqtt()
{
  Serial.println("Ingresa a Config Nodo...");

    
    int section = Sensor.Section; // Asignamos el valor de Sensor.Section a una variable entera

    sprintf(mqtt_topic, "%s%d", MQTT_TOPIC, section); // Concatenamos MQTT_TOPIC con el valor de section y lo almacenamos en mqtt_topic
    
 
}

ModuleConfig MDL = {
  .ID = 1,
  .FlowOnDirection = HIGH,
  .Debounce = 10
};
bool AutoOn = true;
bool MasterOn = false;
const uint16_t LOOP_TIME = 50; // in msec = 20hz
uint32_t LoopLast = LOOP_TIME;
float speedKmH;
