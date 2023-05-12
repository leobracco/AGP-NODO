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
    Sensor.SensorSemilla= json["SensorSemilla"];
    Sensor.Section= json["Section"];
    
    

  
    
 
}
void ConfigNetWork()
 {
    WifiConf.ssid="LB";
    WifiConf.password="1564santiago";
    WifiConf.hostnameOTA="192.168.0.10";
    WifiConf.passwordOTA="tuspass";

   //MQTTConf.brokerAddress="192.168.1.17";   // Dirección IP o nombre de dominio del broker MQTT
   strncpy(MQTTConf.brokerAddress, "192.168.1.17", sizeof(MQTTConf.brokerAddress));
    MQTTConf.port=1883;                 // Puerto del broker MQTT
   strncpy(MQTTConf.clientID, "AGP-NODO-1", sizeof(MQTTConf.clientID));
   strncpy(MQTTConf.topic, "/APG/NODO/", sizeof(MQTTConf.topic));
    //MQTTConf.clientID="AGP-NODO-1";        // ID único del cliente MQTT
   // MQTTConf.topic="/AGP/NODO/";           // Tópico MQTT que se utilizará para publicar y suscribirse a los mensajes

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

