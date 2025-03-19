// test/mocks/wire_mock.cpp
#include "MockWireDeclare.h"

// Define fake functions
DEFINE_FAKE_VOID_FUNC(Wire_begin);
DEFINE_FAKE_VOID_FUNC(Wire_setClock, unsigned long);
DEFINE_FAKE_VOID_FUNC(Wire_beginTransmission, uint8_t);
DEFINE_FAKE_VALUE_FUNC(uint8_t, Wire_endTransmission, bool);
DEFINE_FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_3args, uint8_t, uint8_t, bool);
DEFINE_FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_2args, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(int, Wire_available);
DEFINE_FAKE_VALUE_FUNC(int, Wire_read);
DEFINE_FAKE_VOID_FUNC(Wire_write_uint8, uint8_t);
DEFINE_FAKE_VOID_FUNC(Wire_write_buffer, const uint8_t*, size_t);

// Define the global Wire instance
MockWire Wire;