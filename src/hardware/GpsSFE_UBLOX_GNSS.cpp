/**
 * @file GpsSFE_UBLOX_GNSS.cpp
 * @brief Implementation of GpsSFE_UBLOX_GNSS class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#include "GpsSFE_UBLOX_GNSS.h"

GpsSFE_UBLOX_GNSS::GpsSFE_UBLOX_GNSS() : gps(){
    //default constructor
}

bool GpsSFE_UBLOX_GNSS::begin() {
    return gps.begin();
}

void GpsSFE_UBLOX_GNSS::setI2COutput(uint8_t comType) {
    gps.setI2COutput(comType);
}

bool GpsSFE_UBLOX_GNSS::setNavigationFrequency(uint8_t navFreq) {
    return gps.setNavigationFrequency(navFreq);
}

void GpsSFE_UBLOX_GNSS::setAutoPVT(bool autoPVT) {
    gps.setAutoPVT(autoPVT);
}

uint8_t GpsSFE_UBLOX_GNSS::getNavigationFrequency() {
    return gps.getNavigationFrequency();
}

uint8_t GpsSFE_UBLOX_GNSS::getMeasurementRate() {
    return gps.getMeasurementRate();
}

uint8_t GpsSFE_UBLOX_GNSS::getNavigationRate() {
    return gps.getNavigationRate();
}

int16_t GpsSFE_UBLOX_GNSS::getATTroll() {
    return gps.getATTroll();
}

int16_t GpsSFE_UBLOX_GNSS::getATTpitch() {
    return gps.getATTpitch();
}

int16_t GpsSFE_UBLOX_GNSS::getATTheading() {
    return gps.getATTheading();
}

void GpsSFE_UBLOX_GNSS::setPacketCfgPayloadSize(uint16_t payloadSize) {
    gps.setPacketCfgPayloadSize(payloadSize);
}

uint8_t GpsSFE_UBLOX_GNSS::getSIV() {
    return gps.getSIV();
}

uint8_t GpsSFE_UBLOX_GNSS::getFixType() {
    return gps.getFixType();
}

bool GpsSFE_UBLOX_GNSS::getPVT() {
    return gps.getPVT();
}

bool GpsSFE_UBLOX_GNSS::getGnssFixOk() {
    return gps.getGnssFixOk();
}

long GpsSFE_UBLOX_GNSS::getAltitude() {
    return gps.getAltitude();
}

long GpsSFE_UBLOX_GNSS::getLongitude() {
    return gps.getLongitude();
}

long GpsSFE_UBLOX_GNSS::getLatitude() {
    return gps.getLatitude();
}

uint8_t GpsSFE_UBLOX_GNSS::getHour() {
    return gps.getHour();
}

uint8_t GpsSFE_UBLOX_GNSS::getMinute() {
    return gps.getMinute();
}

uint8_t GpsSFE_UBLOX_GNSS::getSecond() {
    return gps.getSecond();
}

bool GpsSFE_UBLOX_GNSS::getDateValid() {
    return gps.getDateValid();
}

bool GpsSFE_UBLOX_GNSS::getTimeValid() {
    return gps.getTimeValid();
}

bool GpsSFE_UBLOX_GNSS::getTimeFullyResolved() {
    return gps.getTimeFullyResolved();
}

bool GpsSFE_UBLOX_GNSS::powerOffWithInterrupt(uint32_t durationInMs, uint32_t wakeupSources, bool forceWhileUsb /*= true*/) {
    // Directly call the underlying library function with the provided arguments.
    // The SFE library function has a 'maxWait' parameter which we can use a default for here,
    // or you could add it to the interface if needed. Using default for now.
    // Note: The SFE library uses defaultMaxWait (often 1100ms)
    return gps.powerOffWithInterrupt(durationInMs, wakeupSources, forceWhileUsb); // Pass arguments directly
}

Isfe_ublox_status_e GpsSFE_UBLOX_GNSS::sendCommand(IUbxPacket *outgoingUBX, uint16_t maxWait) {

    if (outgoingUBX == nullptr) {
        return INVALID_ARG; // Or another appropriate error status
    }

    // 1. Create a temporary ubxPacket structure compatible with the SFE library
    ubxPacket internalPacket;

    // 2. Copy the essential fields from the interface packet (IUbxPacket)
    //    to the internal packet (ubxPacket).
    internalPacket.cls = outgoingUBX->cls;
    internalPacket.id = outgoingUBX->id;
    internalPacket.len = outgoingUBX->len;
    internalPacket.payload = outgoingUBX->payload; // Share the payload pointer

    // These fields are typically managed by the SFE library during sending (checksum)
    // or receiving (counter, startingSpot, validity flags), so initialize reasonably.
    internalPacket.counter = 0;
    internalPacket.startingSpot = 0;
    internalPacket.checksumA = 0; // Will be calculated by gps.sendCommand()
    internalPacket.checksumB = 0; // Will be calculated by gps.sendCommand()
    internalPacket.valid = SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED; // Status set upon reception if used
    internalPacket.classAndIDmatch = SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED; // Status set upon reception if used

    // 3. Call the underlying SFE library's sendCommand function, passing the address
    //    of the temporary internalPacket.
    sfe_ublox_status_e status = gps.sendCommand(&internalPacket, maxWait);

    // 4. Translate the SFE library's status enum (sfe_ublox_status_e)
    //    to the interface's status enum (Isfe_ublox_status_e).
    Isfe_ublox_status_e returnStatus;
    switch (status) {
        case SFE_UBLOX_STATUS_SUCCESS:          returnStatus = SUCCESS; break;
        case SFE_UBLOX_STATUS_FAIL:             returnStatus = FAIL; break;
        case SFE_UBLOX_STATUS_CRC_FAIL:         returnStatus = CRC_FAIL; break;
        case SFE_UBLOX_STATUS_TIMEOUT:          returnStatus = TIMEOUT; break;
        case SFE_UBLOX_STATUS_COMMAND_NACK:     returnStatus = COMMAND_NACK; break;
        case SFE_UBLOX_STATUS_OUT_OF_RANGE:     returnStatus = OUT_OF_RANGE; break;
        case SFE_UBLOX_STATUS_INVALID_ARG:      returnStatus = INVALID_ARG; break;
        case SFE_UBLOX_STATUS_INVALID_OPERATION:returnStatus = INVALID_OPERATION; break;
        case SFE_UBLOX_STATUS_MEM_ERR:          returnStatus = MEM_ERR; break;
        case SFE_UBLOX_STATUS_HW_ERR:           returnStatus = HW_ERR; break;
        case SFE_UBLOX_STATUS_DATA_SENT:        returnStatus = DATA_SENT; break;
        case SFE_UBLOX_STATUS_DATA_RECEIVED:    returnStatus = DATA_RECEIVED; break;
        case SFE_UBLOX_STATUS_I2C_COMM_FAILURE: returnStatus = I2C_COMM_FAILURE; break;
        case SFE_UBLOX_STATUS_DATA_OVERWRITTEN: returnStatus = DATA_OVERWRITTEN; break;
        default:
            // Handle any unexpected status from the SFE library, map to a generic failure.
            returnStatus = FAIL;
            break;
    }

    // 5. Return the translated status defined by the IGps interface.
    return returnStatus;
}