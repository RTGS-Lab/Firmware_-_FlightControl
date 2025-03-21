// test/mocks/mock_wire_defs.h
#ifndef WIRE_MOCK_DEFS_H
#define WIRE_MOCK_DEFS_H

#include <cstdint>
#include "fff.h"

// Declare fake functions
DECLARE_FAKE_VOID_FUNC(Wire_begin);
DECLARE_FAKE_VOID_FUNC(Wire_setClock, unsigned long);
DECLARE_FAKE_VOID_FUNC(Wire_beginTransmission, uint8_t);
DECLARE_FAKE_VALUE_FUNC(uint8_t, Wire_endTransmission, bool);
DECLARE_FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_3args, uint8_t, uint8_t, bool);
DECLARE_FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_2args, uint8_t, uint8_t);
DECLARE_FAKE_VALUE_FUNC(int, Wire_available);
DECLARE_FAKE_VALUE_FUNC(int, Wire_read);
DECLARE_FAKE_VOID_FUNC(Wire_write_uint8, uint8_t);
DECLARE_FAKE_VOID_FUNC(Wire_write_buffer, const uint8_t*, size_t);

// Mock Wire class declaration
class MockWire {
public:
    void begin() { Wire_begin(); }
    void setClock(unsigned long clock) { Wire_setClock(clock); }
    void beginTransmission(uint8_t address) { Wire_beginTransmission(address); }
    uint8_t endTransmission(bool sendStop = true) { return Wire_endTransmission(sendStop); }
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop) {
        return Wire_requestFrom_3args(address, quantity, sendStop);
    }
    uint8_t requestFrom(uint8_t address, uint8_t quantity) {
        return Wire_requestFrom_2args(address, quantity);
    }
    int available() { return Wire_available(); }
    int read() { return Wire_read(); }
    void write(uint8_t data) { Wire_write_uint8(data); }
    void write(const uint8_t* data, size_t quantity) { Wire_write_buffer(data, quantity); }
};

extern MockWire Wire;

#endif // WIRE_MOCK_DEFS_H