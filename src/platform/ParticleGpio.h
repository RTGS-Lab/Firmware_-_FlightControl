// src/platform/ParticleGpio.h
#ifndef PARTICLE_GPIO_H
#define PARTICLE_GPIO_H

#include "IGpio.h" // Include the interface definition
#include "Particle.h" // Include the actual Particle header HERE

/**
 * @brief Concrete implementation of IGpio
 */
class ParticleGpio : public IGpio {
public:
    // Constructor/Destructor (often default is fine)
    ParticleGpio() = default;
    ~ParticleGpio() override = default;

    // Implement methods from IGpio
    void pinMode(uint16_t pin, IPinMode mode) override;
    void digitalWrite(uint16_t pin, uint8_t value) override;
    int32_t digitalRead(uint16_t pin) override;
};

#endif // PARTICLE_GPIO_H