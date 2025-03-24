#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions for PCA9634 LED driver
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_begin);
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_begin_address, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_setLEDOutputMode, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_setLEDDriverMode, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_setBrightness, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCA9634_setOutputState, uint8_t, uint8_t);

// Mock class for PCA9634 LED driver
class PCA9634 {
public:
    bool begin() { return PCA9634_begin(); }
    bool begin(uint8_t address) { return PCA9634_begin_address(address); }
    bool setLEDOutputMode(uint8_t ledNum, uint8_t mode) { return PCA9634_setLEDOutputMode(ledNum, mode); }
    bool setLEDDriverMode(uint8_t ledNum, uint8_t mode) { return PCA9634_setLEDDriverMode(ledNum, mode); }
    bool setBrightness(uint8_t ledNum, uint8_t brightness) { return PCA9634_setBrightness(ledNum, brightness); }
    bool setOutputState(uint8_t ledNum, uint8_t state) { return PCA9634_setOutputState(ledNum, state); }
};