#include <Arduino.h>
#include "config.h"
#include <stdlib.h>

#include <rate.h>
#include <control.h>
#include <AutoPID.h>

#define MIN_OUTPUT Motor.MinPWM
#define MAX_OUTPUT Motor.MaxPWM

double input, output, setPoint;
AutoPID pid(&input, &setPoint, &output, MIN_OUTPUT, MAX_OUTPUT, Pid.KP, Pid.KI, Pid.KD);

void setup_pid()
{
  
  //pid.setBangBang(5); // Valor de histéresis para el control bang-bang
  pid.setTimeStep(75); // Intervalo de tiempo entre cada iteración del PID (ms
  pid.setOutputRange(MIN_OUTPUT, MAX_OUTPUT);
  pid.setGains(Pid.KP, Pid.KI, Pid.KD); 
}



void printTuningParameters()
{
  

  //Serial.print("Kp: ");
  Serial.printf("KP: %.5f\n", pid.getKp());
  Serial.printf("KI: %.5f\n", pid.getKi());
  Serial.printf("KD: %.5f\n", pid.getKd());
  Serial.print(", Output: ");
  Serial.println(output);
}



void loop_pid()
{
  setPoint =  Cal.SetPoint;

  
  pid.run();

  //analogWrite(18, output);
}