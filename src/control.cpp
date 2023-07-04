#include <Arduino.h>
#include "config.h"
#include <pid.h>
#include "control.h"
#include "rate.h"
#include <stdlib.h>

void loop_control()
{
    loop_pid();
    if (millis() - LoopLast >= LOOP_TIME)
    {
         Cal.SetPoint = CalcularDosis();

        loop_rate();
      
            
      
        
        LoopLast = millis();
        // GetUPM();
       
        Pid.FlowEnabled =
            ((Cal.SetPoint > 0 && MasterOn) ||
             ((Motor.ControlType == 4) &&
              (Cal.SetPoint > 0)) ||
             (!AutoOn && MasterOn));
        if (Cal.SetPoint > 0)
            analogWrite(18, output);
        else
            analogWrite(18, 0);
    }
}

void setup_control()
{
    pinMode(5, OUTPUT);
    pinMode(18, OUTPUT);
}
float CalcularDosis()
{
    float velocidadMTS = speedKmH * 1000;
    float segundosenCienMetros = 100 * 3600 / velocidadMTS;
    float laborenCienMetros = Cal.Working_Width * 100 / 10000;
    float dosisCienmetros = laborenCienMetros * Cal.dosagePerHectare;
    float dosisenSegundos = 60 * dosisCienmetros / segundosenCienMetros;
    float DosisPulsos = dosisenSegundos / Cal.MeterCal;

    return DosisPulsos;
}