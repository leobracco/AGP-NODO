#include "wifi_connect.h"
#include "config.h"


void connectToWiFi() {
  Serial.print("Conectando a ");
  Serial.println(WifiConf.ssid);

  WiFi.begin(WifiConf.ssid, WifiConf.password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi exitosa");
}
