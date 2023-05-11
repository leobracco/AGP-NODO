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
void Semillas();
void  setup_interrupciones()
{
		Serial.print("PIN:");
		Serial.println(Sensor.FlowPin);
    	attachInterrupt(digitalPinToInterrupt(Sensor.FlowPin), Pulsos, FALLING);
		attachInterrupt(digitalPinToInterrupt(Sensor.SensorSemilla), Semillas, FALLING);
		
}
void ICACHE_RAM_ATTR Pulsos()
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

void ICACHE_RAM_ATTR Semillas()
{
	Serial.println("Cae semilla");
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
	if (PulseCount) // Verifica si se han recibido pulsos del sensor
	{
		noInterrupts(); // Deshabilita las interrupciones para obtener una lectura precisa
		CurrentCount = PulseCount; // Almacena el número de pulsos recibidos
		PulseCount = 0; // Reinicia el contador de pulsos
		CurrentDuration = Duration; // Almacena la duración de los pulsos
		interrupts(); // Habilita las interrupciones

		if (Sensor.UseMultiPulses) // Verifica si el sensor está configurado para enviar múltiples pulsos por medición
		{
			// baja ms/pulso, usar pulsos en un intervalo de tiempo
			if (FullCount) // Verifica si se ha recibido la cantidad completa de pulsos por medición
			{
				PPM = 60000000 / GetAvgDuration(); // Calcula la velocidad del flujo en unidades por minuto
			}
			else
			{
				PPM = 0; // Si no se ha recibido la cantidad completa de pulsos, la velocidad del flujo es cero
			}
		}
		else // El sensor envía un solo pulso por medición
		{
			// alta ms/pulso, usar tiempo para un pulso
			if (CurrentDuration == 0) // Verifica si la duración del pulso es cero
			{
				PPM = 0; // Si la duración del pulso es cero, la velocidad del flujo es cero
			}
			else
			{
				unsigned long tmp = 6000000000 / CurrentDuration; // Calcula la velocidad del flujo en unidades por minuto

				// media olímpica
				Osum += tmp; // Acumula las lecturas de velocidad
				if (Omax < tmp) Omax = tmp; // Almacena la velocidad máxima
				if (Omin > tmp) Omin = tmp; // Almacena la velocidad mínima

				Ocount++; // Incrementa el contador de lecturas
				if (Ocount > 4) // Verifica si se han realizado cuatro lecturas
				{
					Osum -= Omax; // Resta la velocidad máxima
					Osum -= Omin; // Resta la velocidad mínima
					Oave = (float)Osum / 300.0;	// divide entre 3 muestras y divide entre 100 para tener un lugar decimal
					Osum = 0; // Reinicia el acumulador de lecturas
					Omax = 0; // Reinicia la velocidad máxima
					Omin = 5000000000; // Reinicia la velocidad mínima
					Ocount = 0; // Reinicia el contador de lecturas
				}
				PPM = Oave; // Asigna la velocidad promedio al resultado final
			}
		}

			LastPulse = millis(); // Almacena el tiempo de la última medición
	Sensor.TotalPulses += CurrentCount; // Acumula la cantidad total de pulsos contados

}

// Comprobar si no hay flujo de líquido
if (millis() - LastPulse > 2000)
{
	PPM = 0;
	Osum = 0;
	Oave = 0;
	Ocount = 0;
}

// Calcular unidades por minuto
if (Sensor.MeterCal > 0)
{
	Sensor.UPM = (float)PPM / (float)Sensor.MeterCal;
}
else
{
	Sensor.UPM = 0;
}
}
