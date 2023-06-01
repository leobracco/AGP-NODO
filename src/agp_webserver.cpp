#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <agp_webserver.h>
WebServer server(80);

void hexStringToString(const String &hexString, String &result)
{
  result = "";
  for (size_t i = 0; i < hexString.length(); i += 2)
  {
    String hexByte = hexString.substring(i, i + 2);
    char byteValue = strtol(hexByte.c_str(), NULL, 16);
    result += byteValue;
  }
}

void handleScanWiFi()
{
  // Realiza el escaneo de redes Wi-Fi
  int numRedes = WiFi.scanNetworks();

  // Crea un objeto JSON
  StaticJsonDocument<1024> doc;

  // Crea un array para almacenar las redes Wi-Fi
  JsonArray redes = doc.createNestedArray("redes");

  // Agrega los datos de cada red Wi-Fi escaneada al array JSON
  for (int i = 0; i < numRedes; i++)
  {
    JsonObject red = redes.createNestedObject();
    red["SSID"] = WiFi.SSID(i);
    red["RSSI"] = WiFi.RSSI(i);
    red["encryptionType"] = WiFi.encryptionType(i);
  }

  // Convierte el JSON en una cadena
  String jsonStr;
  serializeJson(doc, jsonStr);

  // Envía la respuesta JSON al cliente
  server.send(200, "application/json", jsonStr);
}

void setupServer()
{

  server.on("/", handleRoot);
  server.on("/scanwifi", handleScanWiFi);
  server.begin();
}

void handleRoot()
{
  String css = R"(<style>
* {
  box-sizing: border-box;
}

body {
  font-family: Arial, sans-serif;
  margin: 0;
  padding: 0;
  display: flex;
  align-items: center;
  justify-content: center;
  height: 100vh;
  background-color: #f2f2f2;
}

.form-container {
  max-width: 400px;
  width: 100%;
   margin: 0 auto;
  padding: 1rem;
  background-color: #ffffff;
  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
  border-radius: 5px;
  position: relative;
  overflow: hidden;
}

.form-container h2 {
  text-align: center;
  margin-bottom: 20px;
}

.form-container form {
  transition: transform 0.6s ease-in-out;
}

.form-container.register-form {
  transform: translateX(400px);
}

.form-container.login-form {
  transform: translateX(0);
}

.form-container.register-form .login-form {
  transform: translateX(-400px);
}

.form-container.register-form h2 {
  margin-bottom: 30px;
}

.form-container input[type="text"],
.form-container input[type="password"],
.form-container input[type="email"] {
  width: 100%;
  padding: 10px;
  margin-bottom: 15px;
  border: 1px solid #ccc;
  border-radius: 3px;
}

.form-container button {
  width: 100%;
  padding: 10px;
  background-color: #4caf50;
  color: #ffffff;
  border: none;
  border-radius: 3px;
  cursor: pointer;
}

.form-container button:hover {
  background-color: #45a049;
}

.form-container .toggle-form {
  position: absolute;
  top: 10px;
  right: 10px;
  font-size: 12px;
  text-decoration: underline;
  color: #666;
  cursor: pointer;
}
.imagen {
  display: block;
  margin-left: auto;
  margin-right: auto;
  width: 50%;
}
</style>
)";

  String html = "\r\n\r\n<!DOCTYPE html>\r\n<html lang=\"es\">\r\n<head>\r\n";
  html += "  <meta charset=\"UTF-8\">\r\n";
  html += "  <title>AGP-NODO || CONFIG || AGRO PARALLEL</title>\r\n";
  html += css;
  html += "</head>\r\n";
  html += "<body>\r\n";
  html += "<div id=\"wifi-form-wrap\" class=\"form-container\" >\r\n";
  html += "<img class=\"imagen\" src='data:image/png;base64,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'>";
  html += "  <h2>Configuracion de Red</h2>\r\n";
  html += "  <form id=\"wifi-form\" class=\"login-form\" >\r\n";
  html += "    <p>\r\n";
  html += "      <label for=\"networks\">Redes disponibles:</label>\r\n";
  html += "      <select id=\"networks\" name=\"networks\">\r\n";
  html += "      </select>\r\n";
  html += "    </p>\r\n";
  html += "    <p>\r\n";
  html += "      <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Password\" required><i class=\"validation\"><span></span><span></span></i>\r\n";
  html += "    </p>\r\n";
  html += "    <p>\r\n";
  html += "      <button type=\"submit\" id=\"Conectar\" value=\"Conectar\"></button>\r\n";
  html += "    </p>\r\n";
  html += "<div id=\"log\"  ></div>\r\n";
  html += "  </form>\r\n";
  html += "</div>\r\n";
  html += "<script>\r\n";
  html += "document.addEventListener(\"DOMContentLoaded\", function() {\r\n";
  html += "  var select = document.getElementById(\"networks\");\r\n";
  html += "  var url = \"/scanwifi\";";
  html += "\r\n";
  html += "  // Agregar mensaje de búsqueda\r\n";
  html += "  var message = document.getElementById(\"log\");\r\n";
  html += "  message.innerHTML = \"Buscando redes...\";\r\n";
  
  html += "\r\n";
  html += "  fetch(url)\r\n";
  html += "    .then(response => response.json())\r\n";
  html += "    .then(data => {\r\n";
  html += "      // Eliminar mensaje de búsqueda\r\n";
  html += "  message.innerHTML = \"Redes encontradas exitosamente...\";\r\n";
  html += "\r\n";
  html += "      data.redes.forEach(function(red) {\r\n";
  html += "        var option = document.createElement(\"option\");\r\n";
  html += "        option.value = red.SSID;\r\n";
  html += "        option.text = red.SSID;\r\n";
  html += "        select.appendChild(option);\r\n";
  html += "      });\r\n";
  html += "    })\r\n";
  html += "    .catch(error => console.error(\"Error:\", error));\r\n";
  html += "\r\n";
  html += "  document.querySelector(\"#wifi-form\").addEventListener(\"submit\", function(event) {\r\n";
  html += "    event.preventDefault();\r\n";
  html += "\r\n";
  html += "    var selectedNetwork = select.value;\r\n";
  html += "    var networkUrl = url + \"?selectedNetwork=\" + encodeURIComponent(selectedNetwork);\r\n";
  html += "\r\n";
  html += "    // Realiza la solicitud GET a la URL con el valor seleccionado\r\n";
  html += "    fetch(networkUrl)\r\n";
  html += "      .then(response => response.text())\r\n";
  html += "      .then(data => {\r\n";
  html += "        console.log(\"Respuesta del servidor:\", data);\r\n";
  html += "        // Aquí puedes procesar la respuesta recibida según tus necesidades\r\n";
  html += "      })\r\n";
  html += "      .catch(error => console.error(\"Error:\", error));\r\n";
  html += "  });\r\n";
  html += "});\r\n";
  html += "</script>\r\n";
  html += "</body>\r\n";
  html += "</html>\r\n\r\n";

  server.send(200, "text/html", html);
}

void handleNotFound()
{
  // Si la ruta no se encontró, devolver código de error 404
  server.send(404, "text/plain", "Recurso no encontrado");
}
