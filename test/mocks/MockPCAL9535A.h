#ifndef MOCK_PCAL9535A_H
#define MOCK_PCAL9535A_H

#include <gmock/gmock.h>
#include "IIOExpander.h"
#include "IWire.h"

class MockPCAL9535A : public IIOExpander {
public:
    // Core functionality
    MOCK_METHOD(int, begin, (), (override));
    
    // Pin control methods
    MOCK_METHOD(int, pinMode, (int Pin, uint8_t State, bool Port), (override));
    MOCK_METHOD(int, pinMode, (int Pin, uint8_t State), (override));
    MOCK_METHOD(int, digitalWrite, (int Pin, bool State, bool Port), (override));
    MOCK_METHOD(int, digitalWrite, (int Pin, bool State), (override));
    MOCK_METHOD(int, digitalRead, (int Pin, bool Port), (override));
    MOCK_METHOD(int, digitalRead, (int Pin), (override));
    
    // Drive strength methods
    MOCK_METHOD(int, pinSetDriveStrength, (int Pin, IDriveStrength State, bool Port), (override));
    MOCK_METHOD(int, pinSetDriveStrength, (int Pin, IDriveStrength State), (override));
    
    // Interrupt methods
    MOCK_METHOD(int, setInterrupt, (int Pin, bool State, bool Port), (override));
    MOCK_METHOD(int, setInterrupt, (int Pin, bool State), (override));
    MOCK_METHOD(int, getInterrupt, (int Pin), (override));
    MOCK_METHOD(uint16_t, getAllInterrupts, (uint8_t Option), (override));
    MOCK_METHOD(uint16_t, getInterruptMask, (), (override));
    MOCK_METHOD(unsigned int, clearInterrupt, (uint8_t age), (override));
    MOCK_METHOD(bool, isInterrupt, (uint8_t age), (override));
    
    // Latch methods
    MOCK_METHOD(int, setLatch, (int Pin, bool State, bool Port), (override));
    MOCK_METHOD(int, setLatch, (int Pin, bool State), (override));
    MOCK_METHOD(uint16_t, getLatch, (), (override));
    
    // Input polarity methods
    MOCK_METHOD(int, setInputPolarity, (int Pin, bool State, bool Port), (override));
    MOCK_METHOD(int, setInputPolarity, (int Pin, bool State), (override));
    MOCK_METHOD(bool, getInputPolarity, (int Pin, bool Port), (override));
    MOCK_METHOD(bool, getInputPolarity, (int Pin), (override));
    
    // Configuration methods
    MOCK_METHOD(int, setIntPinConfig, (int Pin, bool Latch), (override));
    MOCK_METHOD(int, setBusOutput, (uint8_t mode, bool Port), (override));
    MOCK_METHOD(uint8_t, getBusOutput, (), (override));
    
    // Bus read method
    MOCK_METHOD(uint16_t, readBus, (), (override));
    
    // Error handling methods
    MOCK_METHOD(uint16_t, getError, (), (override));
    MOCK_METHOD(uint16_t, clearError, (), (override));
    MOCK_METHOD(void, safeMode, (int state), (override));
    
    // Additional methods
    MOCK_METHOD(uint16_t, readWord, (int Pos, int &Error), (override));
};

#endif // MOCK_PCAL9535A_H