// test/mocks/MockPAC1934.h

#ifndef MOCK_PAC1934_H
#define MOCK_PAC1934_H

#include <gmock/gmock.h>
#include "ICurrentSenseAmplifier.h"

/**
 * @brief Google Mock implementation of ICurrentSenseAmplifier for testing.
 * 
 * This mock allows for unit testing of code that depends on ICurrentSenseAmplifier
 * without requiring actual hardware.
 */
class MockPAC1934 : public ICurrentSenseAmplifier {
public:
    // Mock all methods defined in the interface
    
    // Configuration methods
    MOCK_METHOD(bool, begin, (), (override));
    MOCK_METHOD(bool, setAddress, (uint8_t addr), (override));
    MOCK_METHOD(bool, enableChannel, (uint8_t channel, bool state), (override));
    MOCK_METHOD(bool, setFrequency, (uint8_t frequency), (override));
    MOCK_METHOD(int, getFrequency, (), (override));

    // Measurement direction
    MOCK_METHOD(void, setVoltageDirection, (uint8_t channel, bool bidirectional), (override));
    MOCK_METHOD(void, setCurrentDirection, (uint8_t channel, bool bidirectional), (override));
    MOCK_METHOD(bool, getVoltageDirection, (uint8_t channel), (override));
    MOCK_METHOD(bool, getCurrentDirection, (uint8_t channel), (override));

    // Measurement methods - overload versions
    MOCK_METHOD(float, getBusVoltage, (uint8_t channel, bool average, bool& status), (override));
    MOCK_METHOD(float, getBusVoltage, (uint8_t channel, bool average), (override));
    MOCK_METHOD(float, getSenseVoltage, (uint8_t channel, bool average, bool& status), (override));
    MOCK_METHOD(float, getSenseVoltage, (uint8_t channel, bool average), (override));
    MOCK_METHOD(float, getCurrent, (uint8_t channel, bool average, bool& status), (override));
    MOCK_METHOD(float, getCurrent, (uint8_t channel, bool average), (override));
    MOCK_METHOD(float, getPowerAvg, (uint8_t channel, bool& status), (override));
    MOCK_METHOD(float, getPowerAvg, (uint8_t channel), (override));

    // Status and control
    MOCK_METHOD(uint8_t, update, (uint8_t clear), (override));
    MOCK_METHOD(bool, testOverflow, (), (override));
};

#endif // MOCK_PAC1934_H