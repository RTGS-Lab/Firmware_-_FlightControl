#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "ISystem.h"    // Include the interface definition
#include "Particle.h"   // Include the actual Particle header

class ParticleSystem : public ISystem {
public:
    ParticleSystem() = default;
    ~ParticleSystem() override = default;

    void on(IEventType event, SystemEventHandler handler) override;
    IResetReason resetReason() override;
    uint32_t freeMemory() override;

    // Update the override signature to return IWakeupReason
    IWakeupReason sleep(const particle::SystemSleepConfiguration& config) override;

};

#endif // PARTICLE_SYSTEM_H