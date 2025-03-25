// test/mocks/MockArduino.h
#pragma once

#include <cstdint>
#include "fff.h"

// Define Arduino pin modes
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

// Define pin values
#define HIGH 1
#define LOW 0

// Define common Arduino pins
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19
#define D20 20
#define D21 21
#define D22 22
#define D23 23

// Digital and Analog I/O
DECLARE_FAKE_VOID_FUNC(pinMode, uint8_t, uint8_t);
DECLARE_FAKE_VOID_FUNC(digitalWrite, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(int, digitalRead, uint8_t);
DECLARE_FAKE_VALUE_FUNC(int, analogRead, uint8_t);
DECLARE_FAKE_VOID_FUNC(analogWrite, uint8_t, int);

// Time functions
DECLARE_FAKE_VALUE_FUNC(unsigned long, millis);
DECLARE_FAKE_VALUE_FUNC(unsigned long, micros);
DECLARE_FAKE_VOID_FUNC(delay, unsigned long);
DECLARE_FAKE_VOID_FUNC(delayMicroseconds, unsigned int);

// Misc
DECLARE_FAKE_VOID_FUNC(randomSeed, unsigned long);
DECLARE_FAKE_VALUE_FUNC(long, random, long, long);

// Forward declarations for Serial functions
DECLARE_FAKE_VOID_FUNC(Serial_begin, unsigned long);
DECLARE_FAKE_VALUE_FUNC(int, Serial_available);
DECLARE_FAKE_VALUE_FUNC(int, Serial_read);
DECLARE_FAKE_VOID_FUNC(Serial_print_s, const char*);
DECLARE_FAKE_VOID_FUNC(Serial_print_i, int);
DECLARE_FAKE_VOID_FUNC(Serial_println_s, const char*);
DECLARE_FAKE_VOID_FUNC(Serial_println_i, int);

// Mock class for the Serial object
class MockSerial {
public:
    void begin(unsigned long baud) { Serial_begin(baud); }
    int available() { return Serial_available(); }
    int read() { return Serial_read(); }
    void print(const char* str) { Serial_print_s(str); }
    void print(int val) { Serial_print_i(val); }
    void println(const char* str) { Serial_println_s(str); }
    void println(int val) { Serial_println_i(val); }
};

extern MockSerial Serial;