#include <Arduino.h>
#include "config.h"
#include <stdlib.h>
#include <pid.h>
unsigned long CurrentAdjustTime;
float ErrorPercentLast;
float ErrorPercentCum;
float Integral;
float LastPWM;

int PIDmotor()
{
    float Result = 0;
    float ErrorPercent = 0;
    if (Pid.FlowEnabled && Cal.RateSetting > 0)
    {
        Result = LastPWM;
        ErrorPercent = Pid.RateError / Cal.RateSetting;
        if (abs(ErrorPercent) > (float)Pid.Deadband / 100.0)
        {
            Result += Pid.KP * ErrorPercent / 25.5;
            unsigned long elapsedTime = millis() - CurrentAdjustTime;
            CurrentAdjustTime = millis();
            ErrorPercentCum += ErrorPercent * (elapsedTime * 0.001);
            Integral += Pid.KI * ErrorPercentCum;
            if (Integral > 10)
                Integral = 10;
            if (Integral < -10)
                Integral = -10;
            if (Pid.KI == 0)
            {
                Integral = 0;
                ErrorPercentCum = 0;
            }
            Result += Integral;
            // add in derivative term to dampen effect of the correction.
            Result += (float)Pid.KD * (ErrorPercent - ErrorPercentLast) / (elapsedTime * 0.001) * 0.001;
            ErrorPercentLast = ErrorPercent;
            if (Result > Motor.MaxPWM)
                Result = (float)Motor.MaxPWM;
            if (Result < Motor.MinPWM)
                Result = (float)Motor.MinPWM;
        }
    }
    else
    {
        Integral = 0;
    }

    LastPWM = Result;
    return (int)Result;
}
