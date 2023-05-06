#ifndef MQTT_CONNECT_H
#define MQTT_CONNECT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

void connectToMQTT(WiFiClient& espClient, PubSubClient& client, const char* MQTT_SERVER, const char* mqttUser, const char* mqttPassword);

#endif
