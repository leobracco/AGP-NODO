#include "wifi_connect.h"

void connectToWiFi(const char* WIFI_SSID, const char* WIFI_PASSWORD) {
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi exitosa");
}
