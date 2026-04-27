#ifndef SENDER_H
#define SENDER_H

#include <Arduino.h>

bool send_tcp_line(const char* host, uint16_t port, const String& line);

#endif