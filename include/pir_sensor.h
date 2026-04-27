#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include <Arduino.h>

void pir_init(uint8_t pin);
bool pir_read();

#endif
