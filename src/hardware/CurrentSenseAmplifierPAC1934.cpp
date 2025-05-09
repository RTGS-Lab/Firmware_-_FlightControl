// src/hardware/CurrentSenseAmplifierPAC1934.cpp

#include "CurrentSenseAmplifierPAC1934.h"

CurrentSenseAmplifierPAC1934::CurrentSenseAmplifierPAC1934(float r1, float r2, float r3, float r4, uint8_t addr)
    : pac1934(r1, r2, r3, r4, addr) {
    // Constructor delegates to PAC1934 constructor
}

bool CurrentSenseAmplifierPAC1934::begin() {
    return pac1934.begin();
}

bool CurrentSenseAmplifierPAC1934::setAddress(uint8_t addr) {
    return pac1934.setAddress(addr);
}

bool CurrentSenseAmplifierPAC1934::enableChannel(uint8_t channel, bool state) {
    return pac1934.enableChannel(channel, state);
}

bool CurrentSenseAmplifierPAC1934::setFrequency(uint16_t frequency) {
    // Map the uint16_t frequency to the PAC1934 Frequency enum
    Frequency freq;
    switch (frequency) {
        case 8:
            freq = SPS_8;
            break;
        case 64:
            freq = SPS_64;
            break;
        case 256:
            freq = SPS_256;
            break;
        case 1024:
        default:
            freq = SPS_1024;
            break;
    }
    return pac1934.setFrequency(freq);
}

int CurrentSenseAmplifierPAC1934::getFrequency() {
    return pac1934.getFrequency();
}

void CurrentSenseAmplifierPAC1934::setVoltageDirection(uint8_t channel, bool bidirectional) {
    pac1934.setVoltageDirection(channel, bidirectional);
}

void CurrentSenseAmplifierPAC1934::setCurrentDirection(uint8_t channel, bool bidirectional) {
    pac1934.setCurrentDirection(channel, bidirectional);
}

bool CurrentSenseAmplifierPAC1934::getVoltageDirection(uint8_t channel) {
    return pac1934.getVoltageDirection(channel);
}

bool CurrentSenseAmplifierPAC1934::getCurrentDirection(uint8_t channel) {
    return pac1934.getCurrentDirection(channel);
}

float CurrentSenseAmplifierPAC1934::getBusVoltage(uint8_t channel, bool average, bool& status) {
    return pac1934.getBusVoltage(channel, average, status);
}

float CurrentSenseAmplifierPAC1934::getBusVoltage(uint8_t channel, bool average) {
    return pac1934.getBusVoltage(channel, average);
}

float CurrentSenseAmplifierPAC1934::getSenseVoltage(uint8_t channel, bool average, bool& status) {
    return pac1934.getSenseVoltage(channel, average, status);
}

float CurrentSenseAmplifierPAC1934::getSenseVoltage(uint8_t channel, bool average) {
    return pac1934.getSenseVoltage(channel, average);
}

float CurrentSenseAmplifierPAC1934::getCurrent(uint8_t channel, bool average, bool& status) {
    return pac1934.getCurrent(channel, average, status);
}

float CurrentSenseAmplifierPAC1934::getCurrent(uint8_t channel, bool average) {
    return pac1934.getCurrent(channel, average);
}

float CurrentSenseAmplifierPAC1934::getPowerAvg(uint8_t channel, bool& status) {
    return pac1934.getPowerAvg(channel, status);
}

float CurrentSenseAmplifierPAC1934::getPowerAvg(uint8_t channel) {
    return pac1934.getPowerAvg(channel);
}

uint8_t CurrentSenseAmplifierPAC1934::update(uint8_t clear) {
    return pac1934.update(clear);
}

bool CurrentSenseAmplifierPAC1934::testOverflow() {
    return pac1934.testOverflow();
}