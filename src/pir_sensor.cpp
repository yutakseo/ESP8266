// pir_sensor.cpp
#include <Arduino.h>
#include "pir_sensor.h"

static uint8_t g_pir_pin = 255;

void pir_init(uint8_t pin) {
  g_pir_pin = pin;
  pinMode(g_pir_pin, INPUT_PULLUP);
}

bool pir_read() {
  if (g_pir_pin == 255) return false;
  return digitalRead(g_pir_pin) == HIGH;
}
