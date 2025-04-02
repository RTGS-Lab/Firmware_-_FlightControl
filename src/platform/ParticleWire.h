// src/platform/ParticleWire.h
#ifndef PARTICLE_WIRE_H
#define PARTICLE_WIRE_H

#include "IWire.h" // Include the interface definition
#include "Particle.h" // Include the actual Particle header HERE

/**
 * @brief Concrete implementation of IWire using Particle API.
 */
class ParticleWire: public IWire {
public:
    // Constructor/Destructor (often default is fine)
    ParticleWire() = default;
    ~ParticleWire() override = default;

    // Implement methods from IWire
    void begin() override;
    void setClock(uint32_t speed) override;
    bool isEnabled() override;
    void beginTransmission(int) override;
    uint8_t endTransmission() override;
    size_t write(uint8_t) override;
    int reset() override;

};

#endif // PARTICLE_WIRE_H