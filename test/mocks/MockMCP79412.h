#pragma once

#include "fff.h"
#include <cstdint>
#include <ctime>

// Declare fake functions
DECLARE_FAKE_VALUE_FUNC(bool, MCP79412_begin);
DECLARE_FAKE_VALUE_FUNC(time_t, MCP79412_getTime);
DECLARE_FAKE_VALUE_FUNC(bool, MCP79412_setTime, time_t);

// Mock class for MCP79412 RTC
class MCP79412 {
public:
    bool begin() { return MCP79412_begin(); }
    time_t getTime() { return MCP79412_getTime(); }
    bool setTime(time_t time) { return MCP79412_setTime(time); }
    
    // Additional properties to match the real implementation
    uint8_t numErrors = 0;
    
    // Add more methods as needed for Kestrel tests
};