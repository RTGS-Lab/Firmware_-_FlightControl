#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions for PAC1934 (power monitor)
DECLARE_FAKE_VALUE_FUNC(bool, PAC1934_begin);
DECLARE_FAKE_VALUE_FUNC(float, PAC1934_readVoltage, uint8_t);
DECLARE_FAKE_VALUE_FUNC(float, PAC1934_readCurrent, uint8_t);
DECLARE_FAKE_VALUE_FUNC(float, PAC1934_readPower, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, PAC1934_enableChannel, uint8_t, bool);

// Mock class for PAC1934
class PAC1934 {
public:
    bool begin() { return PAC1934_begin(); }
    float readVoltage(uint8_t channel) { return PAC1934_readVoltage(channel); }
    float readCurrent(uint8_t channel) { return PAC1934_readCurrent(channel); }
    float readPower(uint8_t channel) { return PAC1934_readPower(channel); }
    bool enableChannel(uint8_t channel, bool enable) { return PAC1934_enableChannel(channel, enable); }
};
