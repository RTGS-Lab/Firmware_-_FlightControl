// src/hardware/LedPCA9634.h
#ifndef LED_PCA9634_H
#define LED_PCA9634_H

#include "ILed.h" // Include the interface definition
#include "../../PCA9634/src/PCA9634.h" // Include the actual driver

/**
 * @brief Concrete implementation of ILed using PCA9634 driver
 */
class LedPCA9634 : public ILed {
public:
    // Constructor/Destructor
    explicit LedPCA9634(int address);
    ~LedPCA9634() override = default;

    // Core methods
    int begin() override;
    int sleep(bool State) override;

    // Configuration methods
    int setOutputMode(IOutputMode State) override;
    int setGroupMode(IGroupMode State) override;

    // Group control
    int setGroupBlinkPeriod(uint16_t Period) override;
    int setGroupOnTime(uint16_t Period) override;
    
    // Brightness control
    int setBrightness(uint8_t Pos, float Brightness) override;
    int setBrightnessArray(float Brightness) override;

    // Output state control
    int setOutput(uint8_t Pos, IPortState State) override;
    int setOutputArray(IPortState Val) override;

private:
    PCA9634 driver; // The concrete driver instance
};

#endif // LED_PCA9634_H