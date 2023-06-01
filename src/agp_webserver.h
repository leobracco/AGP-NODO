#ifndef AGP_WEBSERVER_H
#define AGP_WEBSERVER_H

#include <DNSServer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>

void setupServer();
void handleRoot();
void hexStringToString();
extern WebServer server; 
#endif
