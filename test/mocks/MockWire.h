#ifndef MOCK_WIRE_H
#define MOCK_WIRE_H

#include <gmock/gmock.h>
#include "IWire.h" // Include the interface definition

/**
 * @brief Google Mock implementation of IWire for testing.
 */
class MockWire : public IWire {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, setClock, (uint32_t speed), (override));
    MOCK_METHOD(bool, isEnabled, (), (override));
    MOCK_METHOD(void, beginTransmission, (int), (override));
    MOCK_METHOD(uint8_t, endTransmission, (), (override));
    MOCK_METHOD(size_t, write, (uint8_t), (override));
    MOCK_METHOD(int, reset, (), (override));
};

#endif // MOCK_WIRE_H