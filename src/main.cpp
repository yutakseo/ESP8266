#include <Arduino.h>
#include "wifi_set.h"
#include "sensor.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  wifiConnect();
  internetCheck();

  sensorInit();
}

void loop() {
  bool motion = motionRead();

  if (motion) {
    Serial.println("Motion detected");
    ledSet(true);
  } else {
    Serial.println("No motion");
    ledSet(false);
  }

  delay(200);
}