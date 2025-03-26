#ifndef MOCK_MCP79412_H
#define MOCK_MCP79412_H

#include <gmock/gmock.h>
#include "MCP79412.h"

class MockMCP79412 : public MCP79412 {
public:
    MOCK_METHOD(int, begin, (bool exOsc));
    MOCK_METHOD(time_t, getTimeUnix, ());
    MOCK_METHOD(String, getUUIDString, ());
    MOCK_METHOD(int, setMode, (MCP79412::Mode mode));
    MOCK_METHOD(int, enableAlarm, (bool enable, uint8_t alarmNum));
    MOCK_METHOD(uint8_t, readByte, (uint8_t address));
};

#endif // MOCK_MCP79412_H