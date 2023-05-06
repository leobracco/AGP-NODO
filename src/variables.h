#include "ParametrosMotorConfig.h"
#include "ParametrosCalConfig.h"
#include "ParametrosPidConfig.h"
ParametrosPidConfig PidConfig = {
    .FlowPin = 1,
    .DirPin = 2,
    .PWMPin = 3,
    .FlowEnabled = true,
    .RateError = 0.0,
    .UPM = 0.0,
    .pwmSetting = 0,
    .CommTime = 0,
    .InCommand = 0,
    .ControlType = 0,
    .TotalPulses = 0,
    .RateSetting = 0.0,
    .MeterCal = 0.0,
    .ManualAdjust = 0.0,
    .KP = 0.0,
    .KI = 0.0,
    .KD = 0.0,
    .MinPWM = 0,
    .MaxPWM = 255,
    .Deadband = 0,
    .BrakePoint = 0,
    .UseMultiPulses = false,
    .Debounce = 0
};


// Definir los pines del motor
extern CytronMD Cytron_Motor;
extern float speedKmH;
