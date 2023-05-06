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
		LoopLast = millis();
		GetUPM();

		
			Sensor.FlowEnabled =    (millis() - Sensor.CommTime < 4000) && 
                                    ((Sensor.RateSetting > 0 && MasterOn)|| 
                                    ((Sensor.ControlType == 4) && 
                                    (Sensor.RateSetting > 0))||
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
	
		Sensor.RateError = Sensor.RateSetting - Sensor.UPM;

		switch (Sensor.ControlType)
		{
		case 2:
		case 3:
		case 4:
			// motor control
			Sensor.pwmSetting = PIDmotor();
			break;

		default:
			// valve control
			Sensor.pwmSetting = PIDmotor();
			break;
		}
	
}

void ManualControl()
{
	
		Sensor.pwmSetting = Sensor.ManualAdjust;
	
}
void AdjustFlow()
{
    
        
            // motor control
            if (Sensor.FlowEnabled)
            {
                if (Sensor.pwmSetting >= 0)
                {
                    //increase
                    digitalWrite(Sensor.DirPin, MDL.FlowOnDirection);
                    analogWrite(Sensor.PWMPin, Sensor.pwmSetting);
                }
                else
                {
                    //decrease
                    digitalWrite(Sensor.DirPin, !MDL.FlowOnDirection);
                    analogWrite(Sensor.PWMPin, -Sensor.pwmSetting);	// offsets the negative pwm value
                }
            }
            else
            {
                // stop motor
                analogWrite(Sensor.PWMPin, 0);
            }
           
}