/**
 * @file RtcMCP79412.cpp
 * @brief Implementation of the RtcMCP79412 class.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "RtcMCP79412.h"

 RtcMCP79412::RtcMCP79412() : rtc() {
     // Default constructor, delegates to MCP79412 constructor
 }
 
 int RtcMCP79412::begin(bool UseExtOsc) {
     return rtc.begin(UseExtOsc);
 }
 
 int RtcMCP79412::setTime(int Year, int Month, int Day, int DoW, int Hour, int Min, int Sec) {
     return rtc.setTime(Year, Month, Day, DoW, Hour, Min, Sec);
 }
 
 int RtcMCP79412::setTime(int Year, int Month, int Day, int Hour, int Min, int Sec) {
     return rtc.setTime(Year, Month, Day, Hour, Min, Sec);
 }
 
 IRtc::Timestamp RtcMCP79412::getRawTime() {
     MCP79412::Timestamp rtcTimestamp = rtc.getRawTime();
     
     // Convert from MCP79412::Timestamp to IRtc::Timestamp
     IRtc::Timestamp interfaceTimestamp;
     interfaceTimestamp.year = rtcTimestamp.year;
     interfaceTimestamp.month = rtcTimestamp.month;
     interfaceTimestamp.mday = rtcTimestamp.mday;
     interfaceTimestamp.wday = rtcTimestamp.wday;
     interfaceTimestamp.hour = rtcTimestamp.hour;
     interfaceTimestamp.min = rtcTimestamp.min;
     interfaceTimestamp.sec = rtcTimestamp.sec;
     
     return interfaceTimestamp;
 }
 
 time_t RtcMCP79412::getTimeUnix() {
     return rtc.getTimeUnix();
 }
 
 int RtcMCP79412::setMode(Mode Val) {
     MCP79412::Mode mcpMode;
     
     // Convert from IRtc::Mode to MCP79412::Mode
     switch(Val) {
         case Mode::Normal:
             mcpMode = MCP79412::Mode::Normal;
             break;
         case Mode::Inverted:
             mcpMode = MCP79412::Mode::Inverted;
             break;
         default:
             // Default to Normal mode
             mcpMode = MCP79412::Mode::Normal;
             break;
     }
     
     return rtc.setMode(mcpMode);
 }
 
 int RtcMCP79412::setAlarm(unsigned int Seconds, bool AlarmNum) {
     return rtc.setAlarm(Seconds, AlarmNum);
 }
 
 int RtcMCP79412::setMinuteAlarm(unsigned int Offset, bool AlarmNum) {
     return rtc.setMinuteAlarm(Offset, AlarmNum);
 }
 
 int RtcMCP79412::setHourAlarm(unsigned int Offset, bool AlarmNum) {
     return rtc.setHourAlarm(Offset, AlarmNum);
 }
 
 int RtcMCP79412::setDayAlarm(unsigned int Offset, bool AlarmNum) {
     return rtc.setDayAlarm(Offset, AlarmNum);
 }
 
 int RtcMCP79412::enableAlarm(bool State, bool AlarmNum) {
     return rtc.enableAlarm(State, AlarmNum);
 }
 
 int RtcMCP79412::clearAlarm(bool AlarmNum) {
     return rtc.clearAlarm(AlarmNum);
 }
 
 bool RtcMCP79412::readAlarm(bool AlarmNum) {
     return rtc.readAlarm(AlarmNum);
 }
 
 String RtcMCP79412::getUUIDString() {
     return rtc.getUUIDString();
 }
 
 uint8_t RtcMCP79412::readByte(int Reg) {
     return rtc.readByte(Reg);
 }
 
 uint8_t RtcMCP79412::getErrorsArray(uint32_t errors[]) {
     return rtc.getErrorsArray(errors);
 }
 
 int RtcMCP79412::throwError(uint32_t error) {
     return rtc.throwError(error);
 }