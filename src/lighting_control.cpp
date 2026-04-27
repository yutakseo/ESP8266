//lighting_control.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "light_control.h"

void builtin_light(int led_pin) {
  static int duty = 1023;
  static int step = -1;
  static unsigned long prev_time = 0;
  unsigned long now_time = millis();

  if (now_time - prev_time < 10) {
    return;
  }

  prev_time = now_time;

  analogWrite(led_pin, duty);
  duty += step;
  if (duty <= 0) {
    duty = 0;
    step = 1;
  }
  if (duty >= 1023) {
    duty = 1023;
    step = -1;
  }
}

