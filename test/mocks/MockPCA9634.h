// test/mocks/MockPCA9634.h
#ifndef MOCK_PCA9634_H
#define MOCK_PCA9634_H

#include <gmock/gmock.h>
#include "ILed.h" // Include the interface definition

/**
 * @brief Google Mock implementation of ILed for testing
 */
class MockPCA9634 : public ILed {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(int, begin, (), (override));
    MOCK_METHOD(int, sleep, (bool State), (override));
    MOCK_METHOD(int, setOutputMode, (OutputMode State), (override));
    MOCK_METHOD(int, setGroupMode, (GroupMode State), (override));
    MOCK_METHOD(int, setGroupBlinkPeriod, (uint16_t Period), (override));
    MOCK_METHOD(int, setGroupOnTime, (uint16_t Period), (override));
    MOCK_METHOD(int, setBrightness, (uint8_t Pos, float Brightness), (override));
    MOCK_METHOD(int, setBrightnessArray, (float Brightness), (override));
    MOCK_METHOD(int, setOutput, (uint8_t Pos, PortState State), (override));
    MOCK_METHOD(int, setOutputArray, (PortState Val), (override));
};

#endif // MOCK_PCA9634_H