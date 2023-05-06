#ifndef PARAMETROSPIDCONFIG_H
#define PARAMETROSPIDCONFIG_H

struct ParametrosPidConfig
{
	uint8_t FlowPin;
	uint8_t DirPin;
	uint8_t PWMPin;
	bool FlowEnabled;
	float RateError;		// rate error X 1000
	float UPM;				// upm X 1000
	int pwmSetting;
	uint32_t CommTime;
	byte InCommand;			// command byte from RateController
	byte ControlType;		// 0 standard, 1 combo close, 2 motor, 3 motor/weight, 4 fan
	uint32_t TotalPulses;
	float RateSetting;
	float MeterCal;
	float ManualAdjust;
	float KP;
	float KI;
	float KD;
	byte MinPWM;
	byte MaxPWM;
	byte Deadband;
	byte BrakePoint;
	bool UseMultiPulses;	// 0 - time for one pulse, 1 - average time for multiple pulses
	uint8_t Debounce;
};

extern ParametrosPidConfig PidConfig;

#endif



