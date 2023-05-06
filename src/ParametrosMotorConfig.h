#ifndef PARAMETROSMOTORCONFIG_H
#define PARAMETROSMOTORCONFIG_H

struct ParametrosMotorConfig {
  // Definir los parámetros de la sembradora x motor
  float DOSIS_POR_HA = 0; // 
  int MODO = 1; // 
  float VELOCIDAD = 0; // 
  double PWM_MANUAL = 0; // 
  int IDMOTOR = 2;
  int AUTOCAL = 0;
};

extern ParametrosMotorConfig MotorConfig;

#endif
