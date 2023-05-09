#include <Arduino.h>
#include "config.h"
#include <ArduinoJson.h>
// Ejemplo de configuración de sensor
void iniciarLittleFS()
{
  if (LittleFS.begin()) {
        Serial.println(F("done."));
    } else {
        Serial.println(F("fail."));
    }
}

/*DynamicJsonDocument loadConfig(const char* FileName) {
  File configFile = LittleFS.open(FileName, "r");
  if (!configFile) {
    Serial.println("No se puede abrir el archivo en Load");

  }

  DynamicJsonDocument doc(1024);
  auto error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("No se puede parsear el archivo");
 
  }

  configFile.close();
  
 return doc;
}*/
String loadConfig(const char* FileName) {
  String configContent = "";
  File configFile = LittleFS.open(FileName, "r");
  if (!configFile) {
    Serial.println("No se puede abrir el archivo en Load");
  } else {
    while (configFile.available()) {
      configContent += char(configFile.read());
    }
    configFile.close();
  }
  return configContent;
}

bool saveConfig(DynamicJsonDocument& json,const char* FileName) {
 

  File configFile = LittleFS.open(FileName, "w");
  if (!configFile) {
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

void ConfigPid()
{
    Serial.println("Ingresa a Config PID...");
   //DynamicJsonDocument json = loadConfig("/PidConfig.json");
   StaticJsonDocument<768> json;
  
  DeserializationError error = deserializeJson(json, loadConfig("/PidConfig.json"));
  serializeJson(json, Serial); 
if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}
    Sensor.FlowPin = json["FlowPin"];
    Sensor.DirPin = json["DirPin"];
    Sensor.PWMPin = json["PWMPin"];
    Sensor.FlowEnabled = json["FlowEnabled"];
    Sensor.RateError = json["RateError"];
    Sensor.UPM = json["UPM"];
    Sensor.pwmSetting = json["pwmSetting"];
    Sensor.CommTime = json["CommTime"];
    Sensor.InCommand = json["InCommand"];
    Sensor.ControlType = json["ControlType"];
    Sensor.TotalPulses = json["TotalPulses"];
    Sensor.RateSetting = json["RateSetting"];
    Sensor.MeterCal = json["MeterCal"];
    Sensor.ManualAdjust = json["ManualAdjust"];
    Sensor.KP = json["KP"];
    Sensor.KI = json["KI"];
    Sensor.KD = json["KD"];
    Sensor.MinPWM = json["MinPWM"];
    Sensor.MaxPWM = json["MaxPWM"];
    Sensor.Deadband = json["Deadband"];
    Sensor.BrakePoint = json["BrakePoint"];
    Sensor.UseMultiPulses = json["UseMultiPulses"];
    Sensor.Debounce = json["Debounce"];
    Sensor.AnchoLabor= json["AnchoLabor"];
    Sensor.DosisHa= json["DosisHa"];
    Sensor.GramoPulso= json["GramoPulso"];
    Sensor.speedKmH= json["speedKmH"];
    
    
 
}

ModuleConfig MDL = {
  .ID = 1,
  .FlowOnDirection = HIGH,
  .Debounce = 10
};
bool AutoOn = true;
bool MasterOn = true;
const uint16_t LOOP_TIME = 50;      //in msec = 20hz
uint32_t LoopLast = LOOP_TIME;

