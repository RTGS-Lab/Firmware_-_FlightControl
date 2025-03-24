#pragma once

#include "fff.h"
#include <cstdint>
#include <ctime>

// Declare fake functions for u-blox GNSS
DECLARE_FAKE_VALUE_FUNC(bool, GNSS_begin, int);
DECLARE_FAKE_VALUE_FUNC(bool, GNSS_isConnected);
DECLARE_FAKE_VALUE_FUNC(bool, GNSS_setI2COutput, uint8_t);
DECLARE_FAKE_VALUE_FUNC(bool, GNSS_setNavigationFrequency, uint8_t);
DECLARE_FAKE_VALUE_FUNC(long, GNSS_getLatitude);
DECLARE_FAKE_VALUE_FUNC(long, GNSS_getLongitude);
DECLARE_FAKE_VALUE_FUNC(long, GNSS_getAltitude);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getSIV);
DECLARE_FAKE_VALUE_FUNC(uint32_t, GNSS_getTimeOfWeek);
DECLARE_FAKE_VALUE_FUNC(uint16_t, GNSS_getYear);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getMonth);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getDay);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getHour);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getMinute);
DECLARE_FAKE_VALUE_FUNC(uint8_t, GNSS_getSecond);

// Mock class for SFE_UBLOX_GNSS
class SFE_UBLOX_GNSS {
public:
    // Default mock values
    long latitude = 449673925;  // Minneapolis ~44.96°N
    long longitude = -932838386; // Minneapolis ~-93.28°E
    long altitude = 25000;      // 250m above MSL
    uint8_t satellites = 8;     // 8 satellites in view
    
    // Time values
    uint16_t year = 2023;
    uint8_t month = 5;
    uint8_t day = 15;
    uint8_t hour = 10;
    uint8_t minute = 30;
    uint8_t second = 0;
    
    bool begin(int wirePort = 0) { return GNSS_begin(wirePort); }
    bool isConnected() { return GNSS_isConnected(); }
    bool setI2COutput(uint8_t comSettings) { return GNSS_setI2COutput(comSettings); }
    bool setNavigationFrequency(uint8_t navFreq) { return GNSS_setNavigationFrequency(navFreq); }
    
    long getLatitude() { return GNSS_getLatitude(); }
    long getLongitude() { return GNSS_getLongitude(); }
    long getAltitude() { return GNSS_getAltitude(); }
    uint8_t getSIV() { return GNSS_getSIV(); }
    
    uint32_t getTimeOfWeek() { return GNSS_getTimeOfWeek(); }
    uint16_t getYear() { return GNSS_getYear(); }
    uint8_t getMonth() { return GNSS_getMonth(); }
    uint8_t getDay() { return GNSS_getDay(); }
    uint8_t getHour() { return GNSS_getHour(); }
    uint8_t getMinute() { return GNSS_getMinute(); }
    uint8_t getSecond() { return GNSS_getSecond(); }
};