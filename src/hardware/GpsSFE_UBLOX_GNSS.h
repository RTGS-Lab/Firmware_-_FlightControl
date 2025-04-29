/**
 * @file GpsSFE_UBLOX_GNSS.h
 * @brief Concrete implementation of IGPS using SparkFun u-blox GNSS library
 *
 * Adapts the SparkFun u-blox GNSS library to the IGPS interface
 * for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#ifndef GPS_SFE_UBLOX_GNSS_H
#define GPS_SFE_UBLOX_GNSS_H

#include "../../lib/FlightControl-hardware-dependencies/src/IGps.h"
#include "../../lib/SparkFun_u-blox_GNSS_Arduino_Library/src/SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "../../lib/FlightControl-platform-dependencies/src/IWire.h"

/**
 * @brief Concrete implementation of IGPS using SFE_UBLOX_GNSS
 */
class GpsSFE_UBLOX_GNSS : public IGps {
public:
    /**
     * @brief Constructor
     * @param wire IWire instance for I2C communication
     */
     GpsSFE_UBLOX_GNSS();
    ~GpsSFE_UBLOX_GNSS() override = default;

    bool begin() override;
    void setI2COutput(uint8_t comType) override;
    bool setNavigationFrequency(uint8_t navFreq) override;
    void setAutoPVT(bool autoPVT) override;
    uint8_t getNavigationFrequency() override;
    uint8_t getMeasurementRate() override;
    uint8_t getNavigationRate() override;
    int16_t getATTroll() override;
    int16_t getATTpitch() override;
    int16_t getATTheading() override;
    void setPacketCfgPayloadSize(uint16_t payloadSize) override;
    uint8_t getSIV() override;
    uint8_t getFixType() override;
    bool getPVT() override;
    bool getGnssFixOk() override;
    long getAltitude() override;
    long getLongitude() override;
    long getLatitude() override;
    uint8_t getHour() override;
    uint8_t getMinute() override;
    uint8_t getSecond() override;
    bool getDateValid() override;
    bool getTimeValid() override;
    bool getTimeFullyResolved() override;
    bool powerOffWithInterrupt(uint32_t durationInMs, uint32_t wakeupSources, bool forceWhileUsb = true) override;
    Isfe_ublox_status_e sendCommand(IUbxPacket *outgoingUBX, uint16_t maxWait) override;
private:
    SFE_UBLOX_GNSS gps; // The concrete GPS instance
};

#endif // GPS_SFE_UBLOX_GNSS_H