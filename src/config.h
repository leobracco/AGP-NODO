#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <LittleFS.h>
#include <ArduinoJson.h>


extern const uint16_t LOOP_TIME; // in msec = 20hz
extern uint32_t LoopLast;

extern bool AutoOn;
extern bool DEBUG;
extern bool MasterOn;

typedef uint8_t byte;
void iniciarLittleFS();

String loadConfig(const char *FileName);
bool saveConfig(DynamicJsonDocument &json, const char *FileName);
void ConfigPid();
void ConfigCalibracion();
void ConfigMotor();
void ConfigNetWork();
void ConfigureMqtt();
void ConfigureNodo();
void ConfigureWifi();
void ResetConfig();

struct NodoConfig
{
	char IdNodo[20];
	char IdSection[10];
	char IdSeed[10];
	char Version[10];
	bool Active;
	uint8_t SeedPin;		 // Pin del sensor de semilla
};

struct WifiConfig
{
	char SSID[40];
	char Password[40];
	char UrlOTA[100];
};
struct MQTTConfig
{
	char BrokerAddress[40]; // Dirección IP o nombre de dominio del broker MQTT
	char Port[6];			// Puerto del broker MQTT
	char Username[20];		// Nombre de usuario para autenticación en el broker MQTT
	char Password[20];		// Contraseña para autenticación en el broker MQTT
	char ClientID[20];		// ID único del cliente MQTT
};

struct PidConfig
{
	bool FlowEnabled;		 // FLAG que indica si el sensor de flujo está habilitado
	float RateError;		 // Error de la velocidad actual con respecto a la velocidad objetivo
	float ErrorPercent;
	uint8_t Debounce;		 // Tiempo de debounce para el sensor de flujo
	double KP;				 // Constante proporcional del controlador PID
	double KI;				 // Constante integral del controlador PID
	double KD;				 // Constante derivativa del controlador PID
	float Deadband;			 // Banda muerta del controlador PID
	byte BrakePoint;		 // Punto de frenado del controlador PID
	bool UseMultiPulses;	 // FLAG que indica si se están usando múltiples pulsos para medir el flujo
	uint8_t FlowOnDirection; // valor que activa la válvula de flujo o establece la dirección del motor
	uint8_t FlowPin;		 // Pin del sensor de flujo
	int pwmSetting;
	uint32_t PulsePerRev;
	float IntegralUpperLimit;
	float IntegralLowerLimit;
	float DerivativeFilter;
	
};

struct CalConfig
{
	bool Auto;
	float ManualPWM;
  	float SetPoint;
  	float MinimumDose;
   	float Working_Width;
  	float PulsePerRev;
  	float HolesPerPlate;
  	float DosePerUnit;
  	float SeedsPerMeter;
  	float SeedsPerHectare;
  	float KilosPerHectare;
  	float LitersPerHectare;
	bool SampleDose;
	float MaxTurns;
  	double PWMMotor;
	float TotalPulses;
	float UPM;

	
};

struct MotorConfig
{

	byte ControlType; // Tipo de control (0 estándar, 1 combinado, 2 motor, 3 motor/peso, 4 ventilador)
	uint8_t DirPin;	  // Pin de dirección del motor
	uint8_t PWMPin;	  // Pin de velocidad del motor
	float MinPWM;	  // Valor mínimo del PWM
	float MaxPWM;	  // Valor máximo del PWM
};

struct SeedConfig
{

	byte PlantingType; // Tipo de siembra (0 gruesa, 1 fina)
	uint8_t SeedPin;   // Pin del sensor de semilla
	uint8_t Debounce; //Tiempo de debounce para el sensor de semilla

};
extern PidConfig Pid;
extern MotorConfig Motor;
extern CalConfig Cal;
extern NodoConfig Nodo;
extern MQTTConfig MQTTConf;
extern WifiConfig WifiConf;
extern SeedConfig Seed;
extern float speedKmH;
#endif // CONFIG_H
