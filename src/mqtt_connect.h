#ifndef MQTT_CONNECT_H
#define MQTT_CONNECT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

void connectToMQTT(WiFiClient& espClient, PubSubClient& client);

#endif
