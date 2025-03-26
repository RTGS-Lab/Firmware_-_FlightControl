#ifndef MOCK_SFE_UBLOX_GNSS_H
#define MOCK_SFE_UBLOX_GNSS_H

#include <gmock/gmock.h>
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"

// Definitions for protocol types
#define COM_TYPE_UBX 0x01

// UBX classes and message IDs
#define UBX_CLASS_NAV 0x01
#define UBX_NAV_STATUS 0x03
#define UBX_CLASS_CFG 0x06
#define UBX_MON_VER 0x04

// Default payload size
#define MAX_PAYLOAD_SIZE 256

// Mock for SFE_UBLOX_GNSS
class MockSFE_UBLOX_GNSS : public SFE_UBLOX_GNSS {
public:
    MOCK_METHOD(bool, begin, ());
    MOCK_METHOD(void, setI2COutput, (uint8_t comType));
    MOCK_METHOD(bool, setNavigationFrequency, (uint8_t navFreq));
    MOCK_METHOD(void, setAutoPVT, (bool autoPVT));
    MOCK_METHOD(uint8_t, getNavigationFrequency, ());
    MOCK_METHOD(uint8_t, getMeasurementRate, ());
    MOCK_METHOD(uint8_t, getNavigationRate, ());
    MOCK_METHOD(int16_t, getATTroll, ());
    MOCK_METHOD(int16_t, getATTpitch, ());
    MOCK_METHOD(int16_t, getATTheading, ());
    MOCK_METHOD(void, setPacketCfgPayloadSize, (uint16_t payloadSize));
    MOCK_METHOD(uint8_t, sendCommand, (ubxPacket* outgoingUBX, uint16_t maxWait));
};

#endif // MOCK_SFE_UBLOX_GNSS_H