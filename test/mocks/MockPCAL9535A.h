#ifndef MOCK_PCAL9535A_H
#define MOCK_PCAL9535A_H

#include <gmock/gmock.h>
#include "PCAL9535A.h"

class MockPCAL9535A : public PCAL9535A {
public:
    MOCK_METHOD(bool, begin, (TwoWire* wire, uint8_t addr));
    MOCK_METHOD(bool, pinMode, (uint16_t pin, uint8_t mode));
    MOCK_METHOD(bool, digitalWrite, (uint16_t pin, uint8_t value));
    MOCK_METHOD(uint8_t, digitalRead, (uint16_t pin));
    MOCK_METHOD(bool, digitalWritePort, (uint8_t port, uint8_t value));
    MOCK_METHOD(uint8_t, digitalReadPort, (uint8_t port));
};

#endif // MOCK_PCAL9535A_H