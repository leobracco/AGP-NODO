#include <Arduino.h>
#include "config.h"
#include "control.h"
#include "rate.h"
#include <stdlib.h>
#include <pid.h>
void loop_control()
{
    if (millis() - LoopLast >= LOOP_TIME)
    {
        Cal.RateSetting = CalcularDosis();
        // Serial.print("RateSetting");
        // Serial.println(Motor.RateSetting);
        LoopLast = millis();
        GetUPM();

        Pid.FlowEnabled = ((Cal.RateSetting > 0 && MasterOn) ||
                           ((Motor.ControlType == 4) &&
                            (Cal.RateSetting > 0)) ||
                           (!AutoOn && MasterOn));

        AdjustFlow();

        if (AutoOn)
        {
            AutoControl();
        }
        else
        {
            ManualControl();
        }
    }
}

void AutoControl()
{

    Pid.RateError = Cal.RateSetting - Cal.UPM;

    switch (Motor.ControlType)
    {
    case 2:
    case 3:
    case 4:
        // motor control
        Pid.pwmSetting = PIDmotor();
        break;

    default:
        // valve control
        Pid.pwmSetting = PIDmotor();
        break;
    }
}

void ManualControl()
{

    Pid.pwmSetting = Cal.ManualAdjust;
}
void AdjustFlow()
{

    // motor control
    if (Pid.FlowEnabled)
    {
        if (Pid.pwmSetting >= 0)
        {
            // increase
            digitalWrite(Motor.DirPin, Pid.FlowOnDirection);
            analogWrite(Motor.PWMPin, Pid.pwmSetting);
        }
        else
        {
            // decrease
            digitalWrite(Motor.DirPin, !Pid.FlowOnDirection);
            analogWrite(Motor.PWMPin, -Pid.pwmSetting); // offsets the negative pwm value
        }
    }
    else
    {
        // stop motor
        analogWrite(Motor.PWMPin, 0);
    }
}
float CalcularDosis()
{
    float velocidadMTS = speedKmH * 1000;
    float segundosenCienMetros = 100 * 3600 / velocidadMTS;
    float laborenCienMetros = Cal.Working_Width * 100 / 10000;
    float dosisCienmetros = laborenCienMetros * Cal.DosePerUnit;
    float dosisenSegundos = 60 * dosisCienmetros / segundosenCienMetros;
    float DosisPulsos = dosisenSegundos / Cal.DosePerPulse;
    return DosisPulsos;
}