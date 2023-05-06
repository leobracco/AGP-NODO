#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#define WIFI_SSID "nombre_de_la_red_wifi"
#define WIFI_PASSWORD "contraseña_de_la_red_wifi"

#define MQTT_SERVER "direccion_del_servidor_mqtt"
#define MQTT_PORT 1883
#define MQTT_USERNAME "usuario_del_servidor_mqtt"
#define MQTT_PASSWORD "contraseña_del_servidor_mqtt"
#define MQTT_CLIENT_NAME "MOTOR-123"
#define MQTT_TOPIC "/mi/topic"

extern bool AutoOn;
extern bool MasterOn;

typedef uint8_t byte;

 struct ModuleConfig
{
	uint8_t ID=1;
	uint8_t SensorCount;        // up to 2 sensors
	uint8_t	IPpart3;			// IP address, 3rd octet
	uint8_t RelayOnSignal;	    // value that turns on relays
	uint8_t FlowOnDirection;	// sets on value for flow valve or sets motor direction
	uint8_t UseMCP23017;        // 0 use Nano pins for relays, 1 use MCP23017 for relays
	uint8_t Relays[16];
	uint8_t Debounce;			// minimum ms pin change, base debounce
};



  struct SensorConfig 
{
	uint8_t FlowPin;
	uint8_t DirPin;
	uint8_t PWMPin;
	bool FlowEnabled;
	float RateError;		// rate error X 1000
	float UPM;				// upm X 1000
	int pwmSetting;
	uint32_t CommTime;
	byte InCommand;			// command byte from RateController
	byte ControlType;		// 0 standard, 1 combo close, 2 motor, 3 motor/weight, 4 fan
	uint32_t TotalPulses;
	float RateSetting;
	float MeterCal;
	float ManualAdjust;
	float KP;
	float KI;
	float KD;
	byte MinPWM;
	byte MaxPWM;
	byte Deadband;
	byte BrakePoint;
	bool UseMultiPulses;	// 0 - time for one pulse, 1 - average time for multiple pulses
	uint8_t Debounce;
} ;

extern SensorConfig Sensor;
extern ModuleConfig MDL;
#endif // CONFIG_H
