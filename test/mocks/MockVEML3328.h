#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions for VEML3328 (light sensor)
DECLARE_FAKE_VALUE_FUNC(bool, VEML3328_begin);
DECLARE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readRed);
DECLARE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readGreen);
DECLARE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readBlue);
DECLARE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readClear);
DECLARE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readIR);

// Mock class for VEML3328
class VEML3328 {
public:
    bool begin() { return VEML3328_begin(); }
    uint16_t readRed() { return VEML3328_readRed(); }
    uint16_t readGreen() { return VEML3328_readGreen(); }
    uint16_t readBlue() { return VEML3328_readBlue(); }
    uint16_t readClear() { return VEML3328_readClear(); }
    uint16_t readIR() { return VEML3328_readIR(); }
};
