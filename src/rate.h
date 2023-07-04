#ifndef RATE_H
#define RATE_H
#include <PubSubClient.h>
extern PubSubClient client;
void setup_rate();
float read_speed(void);
void loop_rate();
extern double RPM;
extern double PPM;

#endif  // RATE_H
