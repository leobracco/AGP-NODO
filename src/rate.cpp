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
unsigned long lastInterruptTime = 0,lastSecondTime = 0;

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
  unsigned long elapsedTime100 = currentMillis - lastInterruptTime;
  unsigned long elapsedTime1000 = currentMillis - lastSecondTime;

  if (elapsedTime100 >= 100) // Realizar los cálculos cada 100 milisegundos
  {
    // Calcular las RPM
    RPM = (pulseCount / PULSES_PER_REVOLUTION) * (60.0f / (elapsedTime100 / 1000.0f));
    PPM = pulseCount * (60.0f / (elapsedTime100 / 1000.0f));
    input=PPM;
   /*if (DEBUG)
    
    Serial.print("PPM: ");
    Serial.print(PPM);
    Serial.print(" RPM: ");
    Serial.print(RPM);
    Serial.print(" SetPoint: ");
    Serial.print(Cal.SetPoint/600);
    Serial.print(" RateError: ");*/
    Cal.TotalPulses+=pulseCount;
    pulseCount = 0;
    lastInterruptTime = currentMillis;
  }

  if (elapsedTime1000 >= 500) // Realizar las acciones adicionales cada 1 segundo
  {
    SendMotorStatus(client);

    lastSecondTime = currentMillis;
  }
}

