//main.cpp
#include <Arduino.h>
#include "wifi_manager.h"
#include "sender.h"
#include "pir_sensor.h"

namespace {
constexpr const char* kServerHost = "203.255.15.92";
constexpr uint16_t kTcpPort = 22300;

constexpr uint32_t kPirReportIntervalMs = 1000;
constexpr uint8_t kPirPin = D5; // NodeMCU D5 = GPIO14
} // namespace

static bool control_flag = true;
static bool last_pir = false;
static unsigned long last_pir_report_ms = 0;
static unsigned long last_detect_send_ms = 0;

static volatile bool pir_change_event = false;

static void IRAM_ATTR on_pir_change() {
  pir_change_event = true;
}


void setup() {
  Serial.begin(115200);
  delay(500);

  connect_wifi(true);
  ap_setting(false);
  connection_check();

  pir_init(kPirPin);
  last_pir = pir_read();
  Serial.print("PIR initial state: ");
  Serial.println(last_pir ? "HIGH" : "LOW");

  attachInterrupt(digitalPinToInterrupt(kPirPin), on_pir_change, CHANGE);
  control_flag = true;
}

void loop() {
  unsigned long now = millis();
  if (now - last_pir_report_ms >= kPirReportIntervalMs) {
    last_pir_report_ms = now;
    bool pir = pir_read();
    Serial.print("PIR(now): ");
    Serial.println(pir ? "MOTION(HIGH)" : "NO MOTION(LOW)");

    // 인터럽트를 놓치거나 PIR이 HIGH로 유지되는 동안에도 전송이 되도록
    // 주기 샘플에서 HIGH면 일정 주기로 Detected!를 보냅니다.
    if (pir && control_flag && (now - last_detect_send_ms >= kPirReportIntervalMs)) {
      last_detect_send_ms = now;
      if (!send_tcp_line(kServerHost, kTcpPort, "Detected!")) {
        Serial.println("send_tcp_line: failed (periodic)");
      }
    }
  }

  // 인터럽트로 감지된 경우에만 처리 (HIGH/LOW 모두 출력)
  if (pir_change_event) {
    pir_change_event = false;

    bool pir = pir_read();
    bool rising = (!last_pir && pir);
    last_pir = pir;

    Serial.print("PIR: ");
    Serial.println(pir ? "MOTION(HIGH)" : "NO MOTION(LOW)");

    // 감지 순간(LOW->HIGH)에만 서버로 1회 전송
    if (rising && control_flag) {
      if (!send_tcp_line(kServerHost, kTcpPort, "Detected!")) {
        Serial.println("send_tcp_line: failed (edge)");
      }
    }
  }

}
