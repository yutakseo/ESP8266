#include <Arduino.h>
#include "sensor.h"

const int pir_pin = D5;
const int led_pin = D6;

void sensorInit() {
  pinMode(pir_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

bool motionRead() {
  return digitalRead(pir_pin) == HIGH;
}

void ledSet(bool state) {
  digitalWrite(led_pin, state ? HIGH : LOW);
}