#include <Arduino.h>

#include "CytronMotorDriver.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PID_AutoTune_v0.h>
#include <LittleFS.h>
#include "parametros.h"


#define DEBUG true
// Configuración de MQTT
const char * mqtt_server = "192.168.0.10";
const int mqtt_port = 1883;
const char * mqtt_topic = "#";
WiFiClient espClient;
PubSubClient client(espClient);
// Definir los pines del motor
CytronMD Cytron_Motor(PWM_DIR, D4, D3); // PWM = Pin 3, DIR = Pin 4.
float speedKmH = 0;
int ESTADO; //0 AUTOMATICO 1 MANUAL 2 CALIBRACION 3 PRUEBA MOTOR
struct ParametrosMotorConfig {
  // Definir los parámetros de la sembradora x motor
  float DOSIS_POR_HA = 0; // 
  int MODO = 1; // 
  float VELOCIDAD = 0; // 
  double PWM_MANUAL = 0; // 
  int IDMOTOR = 2;
  int AUTOCAL = 0;
};
// Definir los parámetros del PID
struct ParametrosCalConfig {

  double PWM_MANUAL = 0;

  int GIROS_MOTOR = 0;
  float PULSOS = 0; // pulsos por vuelta del motor
  float MUESTRA = 0; // pulsos por vuelta del motor
  float GR_PULSO = 0; // pulsos por vuelta del motor
  float ANCHO_LABOR = 0; // ancho en metros

};
// Definir los parámetros del PID
struct ParametrosPidConfig {
  double KP = 0.0009;
  double KI = 0.000;
  double KD = 0.000;
  int SECCION = 1;
  double PWM_MIN = 0;
  double PWM_MAX = 255;
  double PWM_MANUAL = 0;
  double PULSOS = 0;
  double RPM = 0;
  double SETPOINT = 0;
  double PWM_OUT = 0; // minimum ms pin change
};

ParametrosPidConfig PidConfig;
ParametrosMotorConfig MotorConfig;
ParametrosCalConfig CalConfig;

double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp = 4, aggKi = 0.2, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;
unsigned int interrupciones = 0;
// El tiempo de espera
const unsigned long espera = 1000; // Esperamos un segundo antes de calcular las RPM
// Variables para el cálculo de RPM
unsigned long tiempoAnterior = 0;
//AutoPID PID_Motor( &PidConfig.PULSOS, &PidConfig.SETPOINT, & PidConfig.PWM_OUT, PidConfig.PWM_MIN, PidConfig.PWM_MAX, PidConfig.KP, PidConfig.KI, PidConfig.KD);

PID PID_Motor( & PidConfig.PULSOS, & PidConfig.PWM_OUT, & PidConfig.SETPOINT, PidConfig.KP, PidConfig.KI, PidConfig.KD, DIRECT);

PID_ATune aTune( & PidConfig.PULSOS, & PidConfig.PWM_OUT);
// Definir los parámetros del encoder
const byte ENCODER_PIN = D7;
volatile unsigned long encoderCount = 0;
volatile unsigned long lastInterruptTime = 0;
// Definir las constantes de conversión

volatile int pulseCount = 0;
volatile int pulseCountCal = 0;
volatile int pulseCountRev = 0;
double ppm;
unsigned long lastMillis;
unsigned long lastMillisSend;
// Función que se ejecuta cuando hay una interrupción
void ICACHE_RAM_ATTR pulseCounter() {
  pulseCount++;
  pulseCountRev++;
  pulseCountCal++;
  interrupciones++;
}
// Definir el nombre del archivo donde se almacenará el JSON
const char * FILENAME = "/config.json";
DynamicJsonDocument jsonDoc(1024);
void setup() {
  // Inicializar la comunicación serial
  Serial.begin(115200);
  
  iniciarLittleFS();
  ConfigPid();
  delay(50);
  ConfigMotor();
  delay(50);
  ConfigCal();
  delay(100);
  PID_Motor.SetOutputLimits(PidConfig.PWM_MIN, PidConfig.PWM_MAX);
  PID_Motor.SetTunings(PidConfig.KP, PidConfig.KI, PidConfig.KD);
  PID_Motor.SetMode(AUTOMATIC);
  delay(100);

  /************AUTOTUNE*********************/

  aTune.SetNoiseBand(5);
  aTune.SetOutputStep(30);
  aTune.SetLookbackSec(10);

  setup_wifi();

  setup_mqtt();
  //Configurar el encoder
  pinMode(ENCODER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), pulseCounter, RISING);
  lastMillis = millis();
  lastMillisSend = millis();
  //PID_Motor.setTimeStep(50);
  PID_Motor.SetSampleTime(1);

  jsonDoc["SetPoint"] = 0;
  jsonDoc["input"] = 0;
  jsonDoc["ppm"] = 0;
  jsonDoc["rpm"] = 0;
  jsonDoc["pwm"] = 0;
  jsonDoc["idmotor"] = MotorConfig.IDMOTOR;

  jsonDoc["output"] = 0;
}
float rpm = 0;
float pulsos = rpm * 24;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (MotorConfig.AUTOCAL==1) {
    
     Serial.println("Entra en auto:");
    
     byte val = (aTune.Runtime());
    if (val == 1) {

      Serial.println("Empieza a calibrar:");
      Serial.print("KP antes:");
      Serial.println(PidConfig.KP);

      PidConfig.KP = aTune.GetKp();
      Serial.print("KP despues:");
      Serial.print(PidConfig.KP);
      PidConfig.KI = aTune.GetKi();
      PidConfig.KD = aTune.GetKd();

     
      PID_Motor.SetTunings(PidConfig.KP, PidConfig.KI, PidConfig.KD);

      PID_Motor.SetOutputLimits(PidConfig.PWM_MIN, PidConfig.PWM_MAX);
      PID_Motor.SetMode(AUTOMATIC);
      MotorConfig.AUTOCAL=0;
    }
  }
  
  // Función para calcular las RPM
  unsigned long tiempoActual = millis();
  // Si ha pasado suficiente tiempo desde la última medición
  if (tiempoActual - tiempoAnterior >= espera) {
    Serial.print("Inerrupciones:");
    Serial.println(interrupciones);
    // Calculamos las RPM
    float rpm = pulseCount * (60000.0 / espera) / 24.0;
    Serial.print("RPM:");
    Serial.println(rpm);
    // Reseteamos las variables
    interrupciones = 0;
    tiempoAnterior = tiempoActual;

    Serial.print("RPM: "); // Imprimir las RPM
    Serial.println(rpm);
    pulseCount = 0; // Reiniciar el contador de pulsos

    // Devolvemos el valor de las RPM/PPM
    PidConfig.RPM = rpm;
    PidConfig.PULSOS = rpm * 24;
  }
  PidConfig.SETPOINT = CalcularDosis();

  PID_Motor.Compute();
  switch (ESTADO) {
  case 0:
      if(PidConfig.SECCION==1)
      {
        if (MotorConfig.MODO == 1)
        {
          if (speedKmH == 0)
              Cytron_Motor.setSpeed(0);
              else
            Cytron_Motor.setSpeed(PidConfig.PWM_OUT);
        }
      else
        {
          Cytron_Motor.setSpeed(MotorConfig.PWM_MANUAL);
       
        }
      }
      else
      Cytron_Motor.setSpeed(0);
    break;
  case 1:
    Serial.println("Manual");
    break;
  case 2:
    Serial.print("Calibracion");
    Serial.println(int(pulseCountRev / 24));
    Serial.println(CalConfig.GIROS_MOTOR);
    if (int(pulseCountRev / 24) < CalConfig.GIROS_MOTOR) {
      Serial.print("PWM_MANUAL:");
      Serial.println(CalConfig.PWM_MANUAL);
      Cytron_Motor.setSpeed(CalConfig.PWM_MANUAL);
    } else {
      Cytron_Motor.setSpeed(0);
      delay(500);
      ESTADO = 0;
    }
    break;
  case 3:
    Serial.println("Test motores");
    break;
    // y así sucesivamente...
  default:
    Serial.println("Sin coincidencias");
    break;
  }

  jsonDoc["SetPoint"] = PidConfig.SETPOINT;
  jsonDoc["input"] = PidConfig.PULSOS;
  jsonDoc["rmp"] = PidConfig.RPM;
  jsonDoc["pwm"] = PidConfig.PWM_OUT;
  jsonDoc["modo"] = MotorConfig.MODO;

  jsonDoc["output"] = PidConfig.PWM_OUT;
  jsonDoc["pulseCountCal"] = pulseCountCal;
  jsonDoc["DOSIS_POR_HA"] = MotorConfig.DOSIS_POR_HA;
  jsonDoc["KP"] = PidConfig.KP;
  jsonDoc["KI"] = PidConfig.KI;
  jsonDoc["KD"] = PidConfig.KD;
  jsonDoc["pwm_manual"] = MotorConfig.PWM_MANUAL;;
  

  char jsonBuffer[512];
  if (millis() - lastMillisSend >= 1000) {
    serializeJson(jsonDoc, jsonBuffer);
    client.publish("/master/estado/motor/2", jsonBuffer);
    lastMillisSend = millis();
  }
}
float CalcularDosis() {
  float velocidadMTS = speedKmH * 1000;
  float segundosenCienMetros = 100 * 3600 / velocidadMTS;
  float laborenCienMetros = CalConfig.ANCHO_LABOR * 100 / 10000;
  float dosisCienmetros = laborenCienMetros * MotorConfig.DOSIS_POR_HA;
  float dosisenSegundos = 60 * dosisCienmetros / segundosenCienMetros;
  float DosisPulsos = dosisenSegundos / CalConfig.GR_PULSO;
  return DosisPulsos;
}