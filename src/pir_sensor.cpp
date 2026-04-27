// sensor.cpp (PIR)
#include <Arduino.h>
#include "pir_sensor.h"

static uint8_t g_pir_pin = 255;

void pir_init(uint8_t pin) {
  g_pir_pin = pin;
  // 입력이 떠서 항상 LOW로 보이는 상황을 줄이기 위해 풀업을 켭니다.
  // PIR 모듈이 push-pull 출력이면 그대로 동작하고, 배선 문제에도 조금 더 강해집니다.
  pinMode(g_pir_pin, INPUT_PULLUP);
}

bool pir_read() {
  if (g_pir_pin == 255) return false;
  return digitalRead(g_pir_pin) == HIGH;
}
