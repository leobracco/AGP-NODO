#include <Arduino.h>
#include "config.h"
// Ejemplo de configuración de sensor
SensorConfig Sensor = {
    .FlowPin = 5,
    .DirPin = 6,
    .PWMPin = 9,
    .FlowEnabled = true,
    .RateError = 0.0,
    .UPM = 2000.0,
    .pwmSetting = 128,
    .CommTime = 2000,
    .InCommand = 0,
    .ControlType = 2,
    .TotalPulses = 0,
    .RateSetting = 100.0,
    .MeterCal = 1.0,
    .ManualAdjust = 0.0,
    .KP = 0.1,
    .KI = 0.01,
    .KD = 0.0,
    .MinPWM = 64,
    .MaxPWM = 200,
    .Deadband = 2,
    .BrakePoint = 0,
    .UseMultiPulses = false,
    .Debounce = 10
};
ModuleConfig MDL = {
  .ID = 1,
  .FlowOnDirection = HIGH,
  .Debounce = 10
};
bool AutoOn = true;
bool MasterOn = true;
const uint16_t LOOP_TIME = 50;      //in msec = 20hz
uint32_t LoopLast = LOOP_TIME;