#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "wifi_set.h"

const char* ssid = "psm3";
const char* password = "psm0403!";

void wifiConnect() {
  Serial.println("\nConnecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retry = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry++;

    if (retry > 40) {
      Serial.println("\nFailed to connect WiFi");
      return;
    }
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

bool internetCheck() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi is not connected");
    return false;
  }

  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://example.com");
  int http_code = http.GET();
  http.end();

  if (http_code == HTTP_CODE_OK) {
    Serial.println("Internet Connected");
    return true;
  }

  Serial.print("Internet Failed, Code: ");
  Serial.println(http_code);
  return false;
}