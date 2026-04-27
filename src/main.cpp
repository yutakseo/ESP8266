//main.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wifi_manager.h"
#include "light_control.h"
#include "sender.h"
#include "pir_sensor.h"

#define SERVER_HOST "203.255.15.92"
#define TCP_PORT 22300

static bool control_flag = true;

// "5번 핀"을 보드 실크의 D5로 연결한 경우가 많아서 D5(GPIO14)로 맞춥니다.
// 만약 진짜 GPIO5(D1)에 꽂았다면 PIR_PIN을 D1로 바꿔주세요.
static const uint8_t PIR_PIN = D5; // NodeMCU D5 = GPIO14
static bool last_pir = false;

static volatile bool pir_change_event = false;

static void IRAM_ATTR on_pir_change() {
  pir_change_event = true;
}


void setup() {
  Serial.begin(115200);
  delay(500);

  //wifi_setting
  connect_wifi(true);
  ap_setting(false);
  connection_check();

  //light_control
  pinMode(LED_BUILTIN, OUTPUT);
  //sensor_define
  pir_init(PIR_PIN);
  last_pir = pir_read();
  Serial.print("PIR initial state: ");
  Serial.println(last_pir ? "HIGH" : "LOW");

  // 상태 변화(HIGH/LOW)를 모두 보기 위해 CHANGE 인터럽트 사용
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), on_pir_change, CHANGE);
  control_flag = true;

}



void loop() {
  builtin_light(LED_BUILTIN);

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
      send_tcp_line(SERVER_HOST, TCP_PORT, "Detected!");
    }
  }

  // 주기적 "hi" 전송은 PIR 테스트가 끝난 뒤 필요하면 다시 켜세요.
}
