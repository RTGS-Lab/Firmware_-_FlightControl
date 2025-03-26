#include <gmock/gmock.h>
#include "MXC6655.h"

class MockMXC6655 : public MXC6655 {
    public:
        MOCK_METHOD(int, begin, ());
        MOCK_METHOD(float, getAccel, (uint8_t axis, uint8_t range));
};