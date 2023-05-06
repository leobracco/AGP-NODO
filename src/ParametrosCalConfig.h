#ifndef PARAMETROSCALCONFIG_H
#define PARAMETROSCALCONFIG_H

struct ParametrosCalConfig {
  double PWM_MANUAL = 0;
  int GIROS_MOTOR = 0;
  float PULSOS = 0; // pulsos por vuelta del motor
  float MUESTRA = 0; // pulsos por vuelta del motor
  float GR_PULSO = 0; // pulsos por vuelta del motor
  float ANCHO_LABOR = 0; // ancho en metros
};

extern ParametrosCalConfig CalConfig;

#endif
