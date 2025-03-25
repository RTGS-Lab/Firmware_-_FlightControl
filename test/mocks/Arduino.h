#pragma once

// Include our Arduino mocks
#include "MockArduino.h"

// Arduino standard libraries
#include <cstdint>
#include <cstring>
#include <cmath>

// Forward declarations for common Arduino types and functions
// This will silence most Arduino.h include errors
typedef uint8_t byte;

class HardwareSerial {
public:
    void begin(unsigned long baud) {}
    int available() { return 0; }
    int read() { return -1; }
    void print(const char* str) {}
    void print(int val) {}
    void println(const char* str) {}
    void println(int val) {}
};

extern HardwareSerial Serial1;
extern HardwareSerial Serial2;

// Add more Arduino types/functions as needed