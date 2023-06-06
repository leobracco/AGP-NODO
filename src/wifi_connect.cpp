#include "wifi_connect.h"
#include "config.h"

bool connectToWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WifiConf.SSID, WifiConf.Password);
  Serial.print("Conectando al WiFi...");

  unsigned long startTime = millis(); // Tiempo de inicio de la conexión

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);

    // Verificar si ha pasado más de 10 segundos
    if (millis() - startTime > 10000)
    {

      return false;
    }
  }

  Serial.println("\nConectado al WiFi.");
  return true;
}

bool enableAPWiFi()
{

  WiFi.mode(WIFI_AP);

  Serial.print("AP on");

  return true;
}
