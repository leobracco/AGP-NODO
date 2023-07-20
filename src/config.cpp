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
  Pid.Deadband =json["Deadband"];
  Pid.Debounce = json["Debounce"];
  Pid.FlowEnabled = json["FlowEnabled"];
  Pid.FlowOnDirection = json["FlowOnDirection"];
  Pid.FlowPin = json["FlowPin"];
  Pid.KP = json["KP"].as<float>();
  Pid.KI = json["KI"].as<float>();
  Pid.KD = json["KD"].as<float>();
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
 
  
  Cal.Auto = json["Auto"];
  Cal.ManualPWM = json["ManualPWM"];
  Cal.SetPoint = json["SetPoint"];
  Cal.MinimumDose = json["MinimumDose"];
  Cal.Working_Width = json["Working_Width"];
  Cal.PulsePerRev = json["PulsePerRev"];
  Cal.HolesPerPlate= json["HolesPerPlate"];
  Cal.DosePerUnit = Cal.HolesPerPlate / Cal.PulsePerRev;
  Cal.SeedsPerMeter=json["SeedsPerMeter"];
  Cal.SeedsPerHectare=json["SeedsPerHectare"];
  Cal.KilosPerHectare=json["KilosPerHectare"];
  Cal.LitersPerHectare=json["LitersPerHectare"];
  Cal.SampleDose=false;
  Cal.MaxTurns=20;
  Cal.PWMMotor=10;
  Cal.TotalPulses=0;
  Cal.UPM=0;
  Serial.print("Pulsos por rev:");
  Serial.println(Cal.PulsePerRev);



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
  Motor.MinPWM = json["MinPWM"].as<float>();
  Motor.MaxPWM = json["MaxPWM"].as<float>();
}
/**********************Carga las variables del Seed desde el Archivo SeedConfig.json*/
void ConfigSeed()
{
  Serial.println("Ingresa a Config Seed...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/SeedConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Seed.PlantingType = json["PlantingType"];
  Seed.SeedPin = json["SeedPin"];
  Seed.Debounce = json["Debounce"];
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
  Nodo.Active = false;
}
/**********************Carga las variables del Mqtt desde el Archivo MqttConfig.json*/
void ConfigureMqtt()
{
  Serial.println("Ingresa a Config Nodo...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/MqttConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  strcpy(MQTTConf.BrokerAddress, json["BrokerAddress"]);
  strcpy(MQTTConf.Port, json["Port"]);
}
/**********************Carga las variables del Wifi desde el Archivo WifiConfig.json*/
void ConfigureWifi()
{
  Serial.println("Ingresa a Config Wifi...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/WifiConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  strcpy(WifiConf.SSID, json["SSID"]);
  strcpy(WifiConf.Password, json["Password"]);
  // strcpy(WifiConf.UrlOTA, json["UrlOTA"]);
  // strcpy(WifiConf.PasswordOTA, json["PasswordOTA"]);
}
void ResetConfig()
{
  DynamicJsonDocument doc(256);
  DynamicJsonDocument docmqtt(256);

  doc["SSID"] = "-";
  doc["Password"] = "-";
  docmqtt["BrokerAddress"] = "192.168.1.1";
  docmqtt["Port"] = "1883";

  if (saveConfig(doc, "/WifiConfig.json"))
  {
    Serial.print("Config Save");

    strcpy(WifiConf.SSID, doc["SSID"]);
    strcpy(WifiConf.Password, doc["Password"]);
  }
  if (saveConfig(docmqtt, "/MqttConfig.json"))
  {
    Serial.print("ConfigMqtt Save");

    strcpy(MQTTConf.BrokerAddress, docmqtt["BrokerAddress"]);
    strcpy(MQTTConf.Port, docmqtt["Port"]);
  }

  delay(1000);
  ESP.restart();
}
bool AutoOn = true;
bool MasterOn = false;
bool DEBUG = false;
const uint16_t LOOP_TIME = 50; // in msec = 20hz
uint32_t LoopLast = LOOP_TIME;
float speedKmH;
