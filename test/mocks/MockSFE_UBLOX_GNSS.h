/**
 * @file MockSFE_UBLOX_GNSS.h
 * @brief Mock implementation of IGPS for testing
 *
 * This class uses Google Mock to create a testable version of the GPS interface.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#ifndef MOCK_SFE_UBLOX_GNSS_H
#define MOCK_SFE_UBLOX_GNSS_H

#include <gmock/gmock.h>
#include "../../lib/FlightControl-hardware-dependencies/src/IGps.h"

/**
 * @brief Google Mock implementation of IGPS for testing
 */
class MockSFE_UBLOX_GNSS : public IGPS {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(bool, begin, (), (override));
    MOCK_METHOD(void, setI2COutput, (uint8_t comType), (override));
    MOCK_METHOD(bool, setNavigationFrequency, (uint8_t navFreq), (override));
    MOCK_METHOD(void, setAutoPVT, (bool autoPVT), (override));
    MOCK_METHOD(uint8_t, getNavigationFrequency, (), (override));
    MOCK_METHOD(uint8_t, getMeasurementRate, (), (override));
    MOCK_METHOD(uint8_t, getNavigationRate, (), (override));
    MOCK_METHOD(int16_t, getATTroll, (), (override));
    MOCK_METHOD(int16_t, getATTpitch, (), (override));
    MOCK_METHOD(int16_t, getATTheading, (), (override));
    MOCK_METHOD(void, setPacketCfgPayloadSize, (uint16_t payloadSize), (override));
    MOCK_METHOD(uint8_t, getSIV, (), (override));
    MOCK_METHOD(uint8_t, getFixType, (), (override));
    MOCK_METHOD(bool, getPVT, (), (override));
    MOCK_METHOD(bool, getGnssFixOk, (), (override));
    MOCK_METHOD(long, getAltitude, (), (override));
    MOCK_METHOD(long, getLongitude, (), (override));
    MOCK_METHOD(long, getLatitude, (), (override));
    MOCK_METHOD(uint8_t, getHour, (), (override));
    MOCK_METHOD(uint8_t, getMinute, (), (override));
    MOCK_METHOD(uint8_t, getSecond, (), (override));
    MOCK_METHOD(bool, getDateValid, (), (override));
    MOCK_METHOD(bool, getTimeValid, (), (override));
    MOCK_METHOD(bool, getTimeFullyResolved, (), (override));
    MOCK_METHOD(bool, powerOffWithInterrupt, (bool enableInterrupt), (override));
};

#endif // MOCK_SFE_UBLOX_GNSS_H