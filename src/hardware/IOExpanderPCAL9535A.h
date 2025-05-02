// src/hardware/IOExpanderPCAL9535A.h

#ifndef IOEXPANDERPCAL9535A_H
#define IOEXPANDERPCAL9535A_H

#include "../../lib/PCAL9535A_Library/src/PCAL9535A.h" //include the driver of PCAL9535A here
#include "IIOExpander.h" //include the interface of IIOExpander here

/**
 * * @breif Concrete implementation of IIOExpander for PCAL9535A
 */
class IOExpanderPCAL9535A : public IIOExpander {
    public:
        IOExpanderPCAL9535A(int _ADR = PCAL9535A_BASE_ADR);
        ~IOExpanderPCAL9535A() = default;

        int begin() override; // Address argument ignored here, set in constructor

        // Core IO
        int pinMode(int Pin, uint8_t State, bool Port) override;
        int pinMode(int Pin, uint8_t State) override;
        int digitalWrite(int Pin, bool State, bool Port) override;
        int digitalWrite(int Pin, bool State) override;
        int digitalRead(int Pin, bool Port) override;
        int digitalRead(int Pin) override;

        // Specific Features
        int pinSetDriveStrength(int Pin, IDriveStrength State, bool Port) override;
        int pinSetDriveStrength(int Pin, IDriveStrength State) override;

        int setInterrupt(int Pin, bool State, bool Port) override;
        int setInterrupt(int Pin, bool State) override;
        int getInterrupt(int Pin) override;
        uint16_t getAllInterrupts(uint8_t Option) override;
        uint16_t getInterruptMask() override;
        unsigned int clearInterrupt(uint8_t age) override;
        bool isInterrupt(uint8_t age) override;

        int setLatch(int Pin, bool State, bool Port) override;
        int setLatch(int Pin, bool State) override;
        uint16_t getLatch() override;

        int setInputPolarity(int Pin, bool State, bool Port) override;
        int setInputPolarity(int Pin, bool State) override;
        bool getInputPolarity(int Pin, bool Port) override;
        bool getInputPolarity(int Pin) override;
        int setIntPinConfig(int Pin, bool Latch) override;

        int setBusOutput(uint8_t mode, bool Port) override;
        uint8_t getBusOutput() override; // Driver returns uint8_t, maybe add to interface later

        uint16_t readBus() override;

        // Error handling
        uint16_t getError() override;
        uint16_t clearError() override;
        void safeMode(int state) override;

        uint16_t readWord(int Pos, int &Error) override;
    private:
        PCAL9535A pcal9535a;
};

#endif // IOEXPANDERPCAL9535A_H