#ifndef MQTT_CONNECT_H
#define MQTT_CONNECT_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <AutoPID.h>

void connectToMQTT(WiFiClient& espClient, PubSubClient& client);
void SendNodo(PubSubClient& client);
void SendMotorStatus(PubSubClient& client);
void enviarEstructuraMQTT(PubSubClient &client);


#endif
