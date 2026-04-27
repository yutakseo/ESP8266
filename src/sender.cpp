// sender.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "sender.h"

bool send_tcp_line(const char* host, uint16_t port, const String& line) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("send_tcp_line: WiFi not connected");
    return false;
  }

  WiFiClient client;
  client.setTimeout(200);
  if (!client.connect(host, port)) {
    Serial.println("send_tcp_line: connect failed");
    return false;
  }

  client.print(line);
  client.print("\r\n");
  client.flush();
  client.stop();

  return true;
}
