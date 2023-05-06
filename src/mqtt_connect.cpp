#include "mqtt_connect.h"

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectToMQTT(WiFiClient& espClient, PubSubClient& client, const char* MQTT_SERVER, const char* MQTT_USERNAME, const char* MQTT_PASSWORD) {
  client.setClient(espClient);
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Conectando a MQTT Broker...");
    if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD )) {
      Serial.println("Conexión exitosa");
      client.subscribe("test/topic");
    } else {
      Serial.print("Falló la conexión con el error ");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}
