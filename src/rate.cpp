#include "Arduino.h"
#include "config.h"
#include "rate.h"
#include "pid.h"
#include <PubSubClient.h>
#include "mqtt_connect.h"

const int ENCODER_PIN = 21;                 // Pin del encoder
const float PULSES_PER_REVOLUTION = 600.0f; // Pulsos por revolución del encoder
double RPM;
double PPM;
volatile unsigned long pulseCount = 0;
unsigned long lastInterruptTime = 0;

void handleEncoderInterrupt()
{
  pulseCount++;
}

void setup_rate()
{
  pinMode(ENCODER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), handleEncoderInterrupt, RISING);
}

void loop_rate()
{
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - lastInterruptTime;

  if (elapsedTime >= 100) // Realizar los cálculos cada 100 milisegundos
  {
    // Calcular las RPM
    RPM = (pulseCount / PULSES_PER_REVOLUTION) * (60.0f / (elapsedTime / 1000.0f));
    PPM = pulseCount * (60.0f / (elapsedTime / 1000.0f));

    // Resto del código de impresión y manipulación de datos
        //Print PPM ,RPM SetPoint
    Serial.print("PPM: ");
    Serial.print(PPM);
    Serial.print(" RPM: ");
    Serial.print(RPM);
    Serial.print(" SetPoint: ");
    Serial.print(Cal.SetPoint/600);
    Serial.print(" RateError: ");
    Serial.print( Pid.RateError);
    Serial.print(" Debounce: ");
    Serial.println( Pid.Debounce);
    
    Pid.RateError = Cal.SetPoint/600 - RPM;
    pulseCount = 0;
    SendMotorStatus(client);
    lastInterruptTime = currentMillis;
  }
}
