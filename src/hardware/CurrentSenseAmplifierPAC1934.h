// src/hardware/CurrentSenseAmplifierPAC1934.h

#ifndef CURRENT_SENSE_AMPLIFIER_PAC1934_H
#define CURRENT_SENSE_AMPLIFIER_PAC1934_H

#include "ICurrentSenseAmplifier.h"
#include "PAC1934.h" // Include the driver header

/**
 * @brief Concrete implementation of ICurrentSenseAmplifier using PAC1934
 * 
 * Adapts the PAC1934 current sense amplifier driver to the ICurrentSenseAmplifier interface
 * for dependency injection and testing.
 */
class CurrentSenseAmplifierPAC1934 : public ICurrentSenseAmplifier {
public:
    /**
     * @brief Constructor
     * @param r1 Channel 1 sense resistor value in milliohms
     * @param r2 Channel 2 sense resistor value in milliohms
     * @param r3 Channel 3 sense resistor value in milliohms
     * @param r4 Channel 4 sense resistor value in milliohms
     * @param addr I2C address of the PAC1934 chip
     */
    CurrentSenseAmplifierPAC1934(float r1 = 0, float r2 = 0, float r3 = 0, float r4 = 0, uint8_t addr = 0x18);
    ~CurrentSenseAmplifierPAC1934() override = default;

    // Configuration methods
    bool begin() override;
    bool setAddress(uint8_t addr) override;
    bool enableChannel(uint8_t channel, bool state) override;
    bool setFrequency(uint16_t frequency) override;
    int getFrequency() override;

    // Measurement direction
    void setVoltageDirection(uint8_t channel, bool bidirectional) override;
    void setCurrentDirection(uint8_t channel, bool bidirectional) override;
    bool getVoltageDirection(uint8_t channel) override;
    bool getCurrentDirection(uint8_t channel) override;

    // Measurement methods
    float getBusVoltage(uint8_t channel, bool average, bool& status) override;
    float getBusVoltage(uint8_t channel, bool average = false) override;
    float getSenseVoltage(uint8_t channel, bool average, bool& status) override;
    float getSenseVoltage(uint8_t channel, bool average = false) override;
    float getCurrent(uint8_t channel, bool average, bool& status) override;
    float getCurrent(uint8_t channel, bool average = false) override;
    float getPowerAvg(uint8_t channel, bool& status) override;
    float getPowerAvg(uint8_t channel) override;

    // Status and control
    uint8_t update(uint8_t clear = 0) override;
    bool testOverflow() override;

private:
    PAC1934 pac1934; // The underlying PAC1934 driver instance
};

#endif // CURRENT_SENSE_AMPLIFIER_PAC1934_H