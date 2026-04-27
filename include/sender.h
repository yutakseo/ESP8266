#ifndef SENDER_H
#define SENDER_H

#include <Arduino.h>


bool send_json_to_server(const char* host, uint16_t port, const char* path, const String& json_body);

bool send_tcp_line(const char* host, uint16_t port, const String& line);

#endif