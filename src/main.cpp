//main.cpp
#include <Arduino.h>
#include "wifi_set.h"
#include "light_control.h"



void setup() {
  Serial.begin(115200);
  delay(500);

  //wifi_setting
  connect_wifi(true);
  ap_setting(false);
  connection_check();

  //light_control
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  builtin_light(LED_BUILTIN);
}
