#include "Arduino.h"
#include "config.h"
#include "seed.h"
#include "pid.h"
#include <PubSubClient.h>
#include "mqtt_connect.h"

const int SEED_PIN = 21;                 // Pin del encoder
const float PULSES_PER_REVOLUTION = 600.0f; // Pulsos por revolución del encoder
double SPM;
volatile unsigned long seedCount = 0;
unsigned long seedlastInterruptTime = 0,seedlastSecondTime = 0;

void SeedCounter()
{
  seedCount++;
}

void setup_seed()
{
  pinMode(SEED_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SEED_PIN), SeedCounter, RISING);
}


void loop_seed()
{
  unsigned long currentMillis = millis();
  unsigned long elapsedTime100 = currentMillis - seedlastInterruptTime;
  unsigned long elapsedTime1000 = currentMillis - seedlastSecondTime;

  if (elapsedTime100 >= 100) // Realizar los cálculos cada 100 milisegundos
  {
    // Calcular las RPM
    
    SPM = seedCount * (60.0f / (elapsedTime100 / 1000.0f));
   
   /*if (DEBUG)
    
    Serial.print("PPM: ");
    Serial.print(PPM);
    Serial.print(" RPM: ");
    Serial.print(RPM);
    Serial.print(" SetPoint: ");
    Serial.print(Cal.SetPoint/600);
    Serial.print(" RateError: ");*/
    //Cal.TotalPulses+=pulseCount;
    seedCount = 0;
    seedlastInterruptTime = currentMillis;
  }

  /*if (elapsedTime1000 >= 500) // Realizar las acciones adicionales cada 1 segundo
  {
    SendMotorStatus(client);

    lastSecondTime = currentMillis;
  }*/
}

