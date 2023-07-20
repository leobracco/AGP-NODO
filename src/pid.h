#ifndef PID_H
#define PID_H
#include <Arduino.h>
#include "config.h"

#include <PID_v1.h>
extern double input;
extern double output;
extern double setPoint;
extern double KP;
extern double KI;
extern double KD;
//extern AutoPID pid;
//extern PID pid;
//extern void setup_pid();
//extern void loop_pid();
extern void printTuningParameters();
extern int PIDvalve();
extern float PIDmotor();

#endif
