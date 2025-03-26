#ifndef MOCK_PCA9634_H
#define MOCK_PCA9634_H

#include <gmock/gmock.h>
#include "PCA9634.h"

class MockPCA9634 : public PCA9634 {
public:
    MockPCA9634(uint8_t address = 0) : PCA9634(address) {}
    
    MOCK_METHOD(int, begin, ());
    MOCK_METHOD(int, setOutputMode, (OutputMode State));
    MOCK_METHOD(int, setGroupMode, (GroupMode State));
    MOCK_METHOD(int, setOutputArray, (PortState Val));
    MOCK_METHOD(int, setBrightnessArray, (float Brightness));
    MOCK_METHOD(int, setGroupOnTime, (uint16_t Period));
};

#endif // MOCK_PCA9634_H