//wifi_manager.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "wifi_manager.h"

const char* ssid = "psm3";
const char* password = "psm0403!";

const char* ap_ssid = "ESP8266-12E";
const char* ap_password = "12345678";


bool connection_check() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi is not connected");
    return false;
  }
  WiFiClient client;
  HTTPClient http;
  
  if (!http.begin(client, "http://example.com")) {
    Serial.println("HTTP begin failed");
    return false;
  }
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


void connect_wifi(bool control) {
  if (control) {
    Serial.println("\nConnecting to WiFi...");
    WiFi.mode(WIFI_AP_STA);
    //wifi connect
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
}


void ap_setting(bool control) {
  if (control) {
    //wifi ap start
    WiFi.softAP(ap_ssid, ap_password);
  }
}