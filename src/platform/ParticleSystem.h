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
    bool waitForCondition(std::function<bool()> condition, std::chrono::milliseconds timeout) override;

    // Updated sleep signature to use ISleepConfig from the interface
    IWakeupReason sleep(const ISleepConfig& config) override;
};

#endif // PARTICLE_SYSTEM_H