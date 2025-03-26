#include <gmock/gmock.h>
#include "VEML3328.h"

class MockVEML3328 : public VEML3328 {
    public:
        MOCK_METHOD(int, begin, ());
        MOCK_METHOD(int, autoRange, ());
        MOCK_METHOD(float, GetValue, (Channel Param, bool &State));
        MOCK_METHOD(float, GetValue, (Channel Param));
        MOCK_METHOD(float, GetLux, ());
};