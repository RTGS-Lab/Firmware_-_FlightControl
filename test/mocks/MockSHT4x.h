#pragma once

#include "fff.h"
#include <cstdint>

// Constants to match Adafruit's SHT4x
#define SHT4X_HIGH_PRECISION 0
#define SHT4X_MED_PRECISION 1
#define SHT4X_LOW_PRECISION 2

// Declare fake functions
DECLARE_FAKE_VALUE_FUNC(bool, SHT4x_begin);
DECLARE_FAKE_VALUE_FUNC(bool, SHT4x_setPrecision, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, SHT4x_readTemperature);
DECLARE_FAKE_VALUE_FUNC(bool, SHT4x_readHumidity);
DECLARE_FAKE_VALUE_FUNC(bool, SHT4x_getEvent);

// Mock class for Adafruit_SHT4x
class Adafruit_SHT4x {
public:
    float temperature = 25.0f; // Default mock values
    float humidity = 50.0f;
    
    bool begin() { return SHT4x_begin(); }
    bool setPrecision(uint8_t precision) { return SHT4x_setPrecision(precision); }
    float readTemperature() { SHT4x_readTemperature(); return temperature; }
    float readHumidity() { SHT4x_readHumidity(); return humidity; }
    bool getEvent() { return SHT4x_getEvent(); }
};
