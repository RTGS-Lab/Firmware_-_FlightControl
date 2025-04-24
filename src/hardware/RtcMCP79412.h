/**
 * @file RtcMCP79412.h
 * @brief Concrete implementation of IRtc using the MCP79412 real-time clock.
 *
 * Adapts the MCP79412 real-time clock to the IRtc interface
 * for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef RTC_MCP79412_H
 #define RTC_MCP79412_H
 
 #include "IRtc.h"
 #include "MCP79412.h" // Include the actual MCP79412 library
 
 /**
  * @brief Concrete implementation of IRtc using MCP79412
  */
 class RtcMCP79412 : public IRtc {
 public:
     RtcMCP79412();
     ~RtcMCP79412() override = default;
 
     int begin(bool UseExtOsc = false) override;
     int setTime(int Year, int Month, int Day, int DoW, int Hour, int Min, int Sec) override;
     int setTime(int Year, int Month, int Day, int Hour, int Min, int Sec) override;
     Timestamp getRawTime() override;
     time_t getTimeUnix() override;
     int setMode(Mode Val) override;
     int setAlarm(unsigned int Seconds, bool AlarmNum = false) override;
     int setMinuteAlarm(unsigned int Offset, bool AlarmNum = false) override;
     int setHourAlarm(unsigned int Offset, bool AlarmNum = false) override;
     int setDayAlarm(unsigned int Offset, bool AlarmNum = false) override;
     int enableAlarm(bool State = true, bool AlarmNum = false) override;
     int clearAlarm(bool AlarmNum = false) override;
     bool readAlarm(bool AlarmNum = false) override;
     String getUUIDString() override;
     uint8_t readByte(int Reg) override;
     uint8_t getErrorsArray(uint32_t errors[]) override;
     int throwError(uint32_t error) override;
 
 private:
     MCP79412 rtc; // The concrete MCP79412 instance
 };
 
 #endif // RTC_MCP79412_H