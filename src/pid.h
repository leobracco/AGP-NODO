#ifndef PID_H
#define PID_H
#include <Arduino.h>
#include "config.h"
#include <AutoPID.h>
extern double input;
extern double output;
extern double setPoint;
extern double Kp;
extern double Ki;
extern double Kd;
extern AutoPID pid;
extern void setup_pid();
extern void loop_pid();
extern void printTuningParameters();
#endif
