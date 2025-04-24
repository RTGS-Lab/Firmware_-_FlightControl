/**
 * @file MockMCP79412.h
 * @brief Mock implementation of IRtc for testing.
 *
 * This class uses Google Mock to create a testable version of the RTC interface.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef MOCK_MCP79412_H
 #define MOCK_MCP79412_H
 
 #include <gmock/gmock.h>
 #include "IRtc.h"
 
 /**
  * @brief Google Mock implementation of IRtc for testing.
  */
 class MockMCP79412 : public IRtc {
 public:
     // Mock all methods defined in the interface
     MOCK_METHOD(int, begin, (bool UseExtOsc), (override));
     MOCK_METHOD(int, setTime, (int Year, int Month, int Day, int DoW, int Hour, int Min, int Sec), (override));
     MOCK_METHOD(int, setTime, (int Year, int Month, int Day, int Hour, int Min, int Sec), (override));
     MOCK_METHOD(Timestamp, getRawTime, (), (override));
     MOCK_METHOD(time_t, getTimeUnix, (), (override));
     MOCK_METHOD(int, setMode, (Mode Val), (override));
     MOCK_METHOD(int, setAlarm, (unsigned int Seconds, bool AlarmNum), (override));
     MOCK_METHOD(int, setMinuteAlarm, (unsigned int Offset, bool AlarmNum), (override));
     MOCK_METHOD(int, setHourAlarm, (unsigned int Offset, bool AlarmNum), (override));
     MOCK_METHOD(int, setDayAlarm, (unsigned int Offset, bool AlarmNum), (override));
     MOCK_METHOD(int, enableAlarm, (bool State, bool AlarmNum), (override));
     MOCK_METHOD(int, clearAlarm, (bool AlarmNum), (override));
     MOCK_METHOD(bool, readAlarm, (bool AlarmNum), (override));
     MOCK_METHOD(uint8_t, readByte, (int Reg), (override));
     MOCK_METHOD(uint8_t, getErrorsArray, (uint32_t errors[]), (override));
     MOCK_METHOD(int, throwError, (uint32_t error), (override));
 };
 
 #endif // MOCK_MCP79412_H