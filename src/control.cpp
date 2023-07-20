#include <Arduino.h>
#include "config.h"
#include <pid.h>
#include "control.h"
#include "rate.h"
#include <stdlib.h>

void loop_control()
{

    if (millis() - LoopLast >= LOOP_TIME)
    {


        Cal.SetPoint = CalcularDosis();
       
        loop_rate();
        //loop_pid();

        Pid.RateError = Cal.SetPoint - PPM;
        LoopLast = millis();
        // GetUPM();
        Cal.PWMMotor=PIDmotor();
        Pid.FlowEnabled =
            ((Cal.SetPoint > 0 && MasterOn) ||
             ((Motor.ControlType == 4) &&
              (Cal.SetPoint > 0)) ||
             (!AutoOn && MasterOn));
        if (Cal.SetPoint > 0 && Nodo.Active && !Cal.SampleDose)
        {
            // Cal.PWMMotor= PIDmotor();
          
            analogWrite(18,Cal.PWMMotor );
        }
        else
        {
            if (Cal.SampleDose && (Cal.TotalPulses / Cal.PulsePerRev <= Cal.MaxTurns))
            {
                analogWrite(18, Cal.PWMMotor);
            }
            else
            {
                analogWrite(18, 0);
                Cal.SampleDose = false;
            }
        }
    }
}

void setup_control()
{
    pinMode(5, OUTPUT);
    pinMode(18, OUTPUT);
}
float CalcularDosis()
{

    float MetrosEnSegundos = speedKmH / 3.6;
    float UnMetroEnSegundos = 1 / MetrosEnSegundos;
    float SemillasPorSegundo = MetrosEnSegundos * 4;
    float SemillasPorMinuto = SemillasPorSegundo * 60;
    float DosisPPM = SemillasPorMinuto / 20 * 600;

    return DosisPPM;
}