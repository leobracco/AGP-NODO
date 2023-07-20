#include <Arduino.h>
#include "config.h"
#include <stdlib.h>

#include <rate.h>
#include <control.h>


double input, output, setPoint;
// PID pid(&input, &output, &setPoint,  Pid.KP, Pid.KI, Pid.KD, DIRECT);

unsigned long CurrentAdjustTime;
float ErrorPercentLast;
float ErrorPercentCum;
float Integral;
float LastPWM;
float PIDmotor() {
  float Result = 0.0f;
  float ErrorPercent = 0.0f;

  if (Cal.SetPoint > 0.0f) {
    Result = LastPWM;
    ErrorPercent = Pid.RateError / Cal.SetPoint * 10.0f;
    Pid.ErrorPercent = ErrorPercent;

    if (fabs(ErrorPercent) > Pid.Deadband) {

      Result += Pid.KP * ErrorPercent / 25.5f;
      Serial.print("KP: ");
      Serial.print(Pid.KP);
      Serial.print(" ErrorPercent: ");
      Serial.print(ErrorPercent);
      Serial.print(" Result: ");
      Serial.println(Result);
      
      unsigned long elapsedTime = millis() - CurrentAdjustTime;
      CurrentAdjustTime = millis();

      // Anti-windup: Limitar el término integral solo si no se alcanzan los límites de salida
      if (Result >= Motor.MinPWM && Result <= Motor.MaxPWM) {
        Integral += Pid.KI * ErrorPercent * elapsedTime * 0.001f;
        if (Integral > 10.0f) Integral = 10.0f;
        if (Integral < -10.0f) Integral = -10.0f;
        if (Pid.KI == 0.0f) {
          Integral = 0.0f;
        }
      }
      
      // Integral windup: Reset the integral term if the output is saturated
      if (Result < Motor.MinPWM || Result > Motor.MaxPWM) {
        Integral = 0.0f;
        // Si el resultado está fuera de los límites, ajustamos el valor para mantenerlo dentro del rango permitido
        Result = constrain(Result, Motor.MinPWM, Motor.MaxPWM);
      }

      Result += Integral;

      Result += Pid.KD * (ErrorPercent - ErrorPercentLast) / (elapsedTime * 0.001f) * 0.001f;

      ErrorPercentLast = ErrorPercent;
    }
    else {
      Integral = 0.0f;
    }
  }

  LastPWM = Result;
  return (int)Result;
}


/*************************************************************/
/****************VALVULA----------------------****************/
/*************************************************************/
int PIDvalve()
{
    float Result = 0;

    if (Cal.SetPoint > 0)
    {
        float ErrorPercent = Pid.RateError / Cal.SetPoint;

        Pid.ErrorPercent = ErrorPercent;
        if (abs(ErrorPercent) > (float)Pid.Deadband)
        {
            Result = Pid.KP * ErrorPercent;

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

            Result += (float)Pid.KD * (ErrorPercent - ErrorPercentLast) / (elapsedTime * 0.001) * 0.001;

            ErrorPercentLast = ErrorPercent;

            bool IsPositive = (Result > 0);
            Result = abs(Result);

            if (Result < Motor.MinPWM)
            {
                Result = Motor.MinPWM;
            }
            else
            {
                if (abs(ErrorPercent) < (float)Pid.BrakePoint / 100.0)
                {
                    if (Result > Motor.MinPWM * 3.0)
                        Result = Motor.MinPWM * 3.0;
                }
                else
                {
                    if (Result > Motor.MaxPWM)
                        Result = Motor.MaxPWM;
                }
            }

            if (!IsPositive)
                Result *= -1;
        }
        else
        {
            Integral = 0;
        }
    }
    return (int)Result;
}
