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

bool GpsSFE_UBLOX_GNSS::powerOffWithInterrupt(bool enableInterrupt) {
    return gps.powerOffWithInterrupt(enableInterrupt);
}