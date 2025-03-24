// test/mocks/MockArduino.cpp
#include "MockArduino.h"

// Define fake functions
DEFINE_FAKE_VOID_FUNC(pinMode, uint8_t, uint8_t);
DEFINE_FAKE_VOID_FUNC(digitalWrite, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(int, digitalRead, uint8_t);
DEFINE_FAKE_VALUE_FUNC(int, analogRead, uint8_t);
DEFINE_FAKE_VOID_FUNC(analogWrite, uint8_t, int);

// Time functions
DEFINE_FAKE_VALUE_FUNC(unsigned long, millis);
DEFINE_FAKE_VALUE_FUNC(unsigned long, micros);
DEFINE_FAKE_VOID_FUNC(delay, unsigned long);
DEFINE_FAKE_VOID_FUNC(delayMicroseconds, unsigned int);

// Misc
DEFINE_FAKE_VOID_FUNC(randomSeed, unsigned long);
DEFINE_FAKE_VALUE_FUNC(long, random, long, long);

// Serial functions
DEFINE_FAKE_VOID_FUNC(Serial_begin, unsigned long);
DEFINE_FAKE_VALUE_FUNC(int, Serial_available);
DEFINE_FAKE_VALUE_FUNC(int, Serial_read);
DEFINE_FAKE_VOID_FUNC(Serial_print_s, const char*);
DEFINE_FAKE_VOID_FUNC(Serial_print_i, int);
DEFINE_FAKE_VOID_FUNC(Serial_println_s, const char*);
DEFINE_FAKE_VOID_FUNC(Serial_println_i, int);

// Define the global Serial instance
MockSerial Serial;