#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#define WIFI_SSID "LB"
#define WIFI_PASSWORD "1564santiago"





extern const uint16_t LOOP_TIME;      //in msec = 20hz
extern uint32_t LoopLast;

extern bool AutoOn;
extern bool MasterOn;
extern char mqtt_topic[50];
typedef uint8_t byte;
void iniciarLittleFS();
//DynamicJsonDocument loadConfig(const char* FileName);
String loadConfig(const char* FileName);
bool saveConfig(DynamicJsonDocument& json, const char* FileName);
void ConfigPid();
struct ModuleConfig
{
    uint8_t ID;                // ID del módulo
    uint8_t FlowOnDirection;   // valor que activa la válvula de flujo o establece la dirección del motor
    uint8_t Debounce;          // tiempo mínimo en ms para considerar que hay un cambio en el estado del pin (debounce)
};



 struct SensorConfig {
	uint8_t FlowPin;         // Pin del sensor de flujo
	uint8_t DirPin;          // Pin de dirección del motor
	uint8_t PWMPin;          // Pin de velocidad del motor
	bool FlowEnabled;        // FLAG que indica si el sensor de flujo está habilitado
	float RateError;        // Error de la velocidad actual con respecto a la velocidad objetivo
	float UPM;               // Unidades por minuto (UPM) de la velocidad actual del motor
	int pwmSetting;         // Configuración del PWM
	uint32_t CommTime;       // Tiempo de la última comunicación con el controlador de velocidad
	byte InCommand;         // Comando del controlador de velocidad recibido
	byte ControlType;       // Tipo de control (0 estándar, 1 combinado, 2 motor, 3 motor/peso, 4 ventilador)
	uint32_t TotalPulses;    // Total de pulsos del sensor de flujo
	float RateSetting;      // Velocidad objetivo del motor en UPM
	float MeterCal;         // Calibración del medidor
	float ManualAdjust;     // Ajuste manual de la velocidad
	float KP;               // Constante proporcional del controlador PID
	float KI;               // Constante integral del controlador PID
	float KD;               // Constante derivativa del controlador PID
	byte MinPWM;            // Valor mínimo del PWM
	byte MaxPWM;            // Valor máximo del PWM
	byte Deadband;          // Banda muerta del controlador PID
	byte BrakePoint;        // Punto de frenado del controlador PID
	bool UseMultiPulses;    // FLAG que indica si se están usando múltiples pulsos para medir el flujo
	uint8_t Debounce;       // Tiempo de debounce para el sensor de flujo
    float AnchoLabor;
    float DosisHa;
    float GramoPulso;
    float speedKmH;
	uint8_t SensorSemilla;
	int Section;
};


extern SensorConfig Sensor;

extern ModuleConfig MDL;

// Definir los pines del motor

extern float speedKmH;
#endif // CONFIG_H
