#include <Arduino.h>
#include "config.h"
#include <stdlib.h>
unsigned long CurrentAdjustTime;
float ErrorPercentLast;
float ErrorPercentCum;
float Integral;
float LastPWM;

int PIDmotor()
{
    float Result = 0;
    float ErrorPercent = 0;
    if (Sensor.FlowEnabled && Sensor.RateSetting > 0)
    {
        Result = LastPWM;
        ErrorPercent = Sensor.RateError / Sensor.RateSetting;
        if (abs(ErrorPercent) > (float)Sensor.Deadband / 100.0)
        {
            Result += Sensor.KP * ErrorPercent / 25.5;
            unsigned long elapsedTime = millis() - CurrentAdjustTime;
            CurrentAdjustTime = millis();
            ErrorPercentCum += ErrorPercent * (elapsedTime * 0.001);
            Integral += Sensor.KI * ErrorPercentCum;
            if (Integral > 10) Integral = 10;
            if (Integral < -10) Integral = -10;
            if (Sensor.KI == 0)
            {
                Integral = 0;
                ErrorPercentCum = 0;
            }
            Result += Integral;
            //add in derivative term to dampen effect of the correction.
            Result += (float)Sensor.KD * (ErrorPercent - ErrorPercentLast) / (elapsedTime * 0.001) * 0.001;
            ErrorPercentLast = ErrorPercent;
            if (Result > Sensor.MaxPWM) Result = (float)Sensor.MaxPWM;
            if (Result < Sensor.MinPWM) Result = (float)Sensor.MinPWM;
        }
    }
    else
    {
        Integral = 0;
    }

    LastPWM = Result;
    return (int)Result;
}
