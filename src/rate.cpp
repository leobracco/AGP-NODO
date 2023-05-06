#include "Arduino.h"
#include "config.h"
#include <stdlib.h>
volatile unsigned long Duration;
volatile unsigned long Durations[12];
volatile byte DurCount;
volatile unsigned long avDurs;
volatile int avgPulses = 12;

volatile unsigned long PulseCount;
volatile bool FullCount;

uint32_t LastPulse;

uint32_t RateInterval;
uint32_t RateTimeLast;
uint32_t PWMTimeLast;

unsigned long CurrentCount;
uint32_t CurrentDuration;

unsigned long PPM;		// pulse per minute * 100
unsigned long Osum;
unsigned long Omax;
unsigned long Omin;
byte Ocount;
float Oave;

void GetUPMflow();
void Pulsos();
void ICACHE_RAM_ATTR setup_interrupciones()
{
    	attachInterrupt(digitalPinToInterrupt(Sensor.FlowPin), Pulsos, FALLING);
}
void Pulsos()
{
	static unsigned long PulseTime;
	unsigned long micronow;
	unsigned long dur;

	micronow = micros();
	if (PulseTime > micronow)
	{
		dur = micronow - PulseTime;
	}
	else
	{
		dur = 0xFFFFFFFF + micronow - PulseTime;
	}

	if (dur > 1000000)
	{
		// the component was off so reset the values
		avDurs = 0;
		dur = 50000;
		for (int i = 0; i < (avgPulses - 1); i++)
		{
			Durations[i] = 0;
		}
		PulseTime = micronow;
		PulseCount++;
		FullCount = false;
	}
	else if (dur > Sensor.Debounce * 1000)
	{
		PulseCount++;
		// check to see if the dur value is too long like an interrupt was missed.
		if ((dur > (1.5 * avDurs)) && (avDurs != 0))
		{
			Durations[DurCount] = avDurs;
			Duration = avDurs;
			dur = avDurs;
		}
		else
		{
			Durations[DurCount] = dur;
			Duration = dur;
		}

		PulseTime = micronow;
		if (DurCount == 0)
		{
			DurCount++;
			avDurs = (Durations[avgPulses - 1] + dur) / 2;
		}
		else if (DurCount < avgPulses)
		{
			DurCount++;
			avDurs = (Durations[DurCount - 1] + dur) / 2;
		}
		else
		{
			DurCount = 0;
			avDurs = (Durations[DurCount - 1] + dur) / 2;
			FullCount = true;
		}
	}
}

void ISR1()
{
	static unsigned long PulseTime;
	unsigned long micronow;
	unsigned long dur;

	micronow = micros();
	if (PulseTime > micronow)
	{
		dur = micronow - PulseTime;
	}
	else
	{
		dur = 0xFFFFFFFF + micronow - PulseTime;
	}

	if (dur > 1000000)
	{
		// the component was off so reset the values
		avDurs = 0;
		dur = 50000;
		for (int i = 0; i < (avgPulses - 1); i++)
		{
			Durations[i] = 0;
		}
		PulseTime = micronow;
		PulseCount++;
		FullCount = false;
	}

	else if (dur > Sensor.Debounce * 1000)
	{
		PulseCount++;
		// check to see if the dur value is too long like an interrupt was missed.
		if ((dur > (1.5 * avDurs)) && (avDurs != 0))
		{
			Durations[DurCount] = avDurs;
			Duration = avDurs;
			dur = avDurs;
		}
		else
		{
			Durations[DurCount] = dur;
			Duration = dur;
		}

		PulseTime = micronow;
		if (DurCount == 0)
		{
			DurCount++;
			avDurs = (Durations[avgPulses - 1] + dur) / 2;
		}
		else if (DurCount < (avgPulses - 1))
		{
			DurCount++;
			avDurs = (Durations[DurCount - 1] + dur) / 2;
		}
		else
		{
			DurCount = 0;
			avDurs = (Durations[DurCount - 1] + dur) / 2;
			FullCount = true;
		}
	}
}

void GetUPM()
{
		if (Sensor.ControlType == 3)
		{
			
			Sensor.UPM = Sensor.MeterCal * (double)Sensor.pwmSetting;
		}
		else
		{
			// use flow meter
			GetUPMflow();
		}
	
}

unsigned long GetAvgDuration()
{
	unsigned long dursum = 0;

	noInterrupts();
	for (int i = 0; i < avgPulses; i++)
	{
		dursum += Durations[i];
	}
	interrupts();
	return dursum / avgPulses;
}

void GetUPMflow()
{
	if (PulseCount)
	{
		noInterrupts();
		CurrentCount = PulseCount;
		PulseCount = 0;
		CurrentDuration = Duration;
		interrupts();

		if (Sensor.UseMultiPulses)
		{
			// low ms/pulse, use pulses over time
			if (FullCount)
			{
				PPM = 60000000 / GetAvgDuration();
			}
			else
			{
				PPM = 0;
			}
		}
		else
		{
			// high ms/pulse, use time for one pulse
			if (CurrentDuration == 0)
			{
				PPM = 0;
			}
			else
			{
				unsigned long tmp = 6000000000 / CurrentDuration;

				// olympic average
				Osum += tmp;
				if (Omax < tmp) Omax = tmp;
				if (Omin > tmp) Omin = tmp;

				Ocount++;
				if (Ocount > 4)
				{
					Osum -= Omax;
					Osum -= Omin;
					Oave = (float)Osum / 300.0;	// divide by 3 samples and divide by 100 for decimal place
					Osum = 0;
					Omax = 0;
					Omin = 5000000000;
					Ocount = 0;
				}
				PPM = Oave;
			}
		}

		LastPulse = millis();
		Sensor.TotalPulses += CurrentCount;
	}

	// check for no flow
	if (millis() - LastPulse > 2000)
	{
		PPM = 0;
		Osum = 0;
		Oave = 0;
		Ocount = 0;
	}

	// units per minute
	if (Sensor.MeterCal > 0)
	{
		Sensor.UPM = (float)PPM / (float)Sensor.MeterCal;
	}
	else
	{
		Sensor.UPM = 0;
	}
}

