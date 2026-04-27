// _sender_.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "sender.h"

bool send_json_to_server(const char* host, uint16_t port, const char* path, const String& json_body) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("send_json_to_server: WiFi not connected");
    return false;
  }

  WiFiClient client;
  HTTPClient http;

  if (!http.begin(client, host, port, path)) {
    Serial.println("send_json_to_server: HTTP begin failed");
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  int code = http.POST((uint8_t*)json_body.c_str(), json_body.length());
  http.end();

  if (code > 0 && code < 400) {
    Serial.print("send_json_to_server: OK, code=");
    Serial.println(code);
    return true;
  }

  Serial.print("send_json_to_server: FAILED, code=");
  Serial.println(code);
  return false;
}

bool send_tcp_line(const char* host, uint16_t port, const String& line) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("send_tcp_line: WiFi not connected");
    return false;
  }

  WiFiClient client;
  // connect()가 실패하면 기본적으로 수 초간 블로킹될 수 있어서
  // 시리얼 출력/센서 루프가 멈춘 것처럼 보일 수 있습니다.
  // 타임아웃을 짧게 잡아 테스트 시 응답성을 유지합니다.
  client.setTimeout(200);
  if (!client.connect(host, port)) {
    Serial.println("send_tcp_line: connect failed");
    return false;
  }

  client.print(line);
  client.print("\r\n");
  client.flush();
  client.stop();

  // 너무 자주 찍히면 시리얼이 스팸처럼 보일 수 있어 전송 성공 로그는 생략
  return true;
}
