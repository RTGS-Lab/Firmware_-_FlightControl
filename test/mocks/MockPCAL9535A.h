#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions
DECLARE_FAKE_VALUE_FUNC(bool, PCAL9535A_begin);
DECLARE_FAKE_VALUE_FUNC(bool, PCAL9535A_pinMode, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCAL9535A_digitalWrite, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(uint8_t, PCAL9535A_digitalRead, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PCAL9535A_begin_address, uint8_t);

// Mock class for PCAL9535A IO expander
class PCAL9535A {
public:
    bool begin() { return PCAL9535A_begin(); }
    bool pinMode(uint8_t pin, uint8_t mode) { return PCAL9535A_pinMode(pin, mode); }
    bool digitalWrite(uint8_t pin, uint8_t value) { return PCAL9535A_digitalWrite(pin, value); }
    uint8_t digitalRead(uint8_t pin) { return PCAL9535A_digitalRead(pin); }
    bool begin(uint8_t address) { return PCAL9535A_begin_address(address); }
    
    // Add more methods as needed for Kestrel tests
};