// src/platform/ParticleCloud.h
#ifndef PARTICLE_CLOUD_H
#define PARTICLE_CLOUD_H

#include "ICloud.h" // Include the interface definition
#include "Particle.h" // Include the actual Particle header HERE

/**
 * @brief Concrete implementation of ICloud for Particle platform
 */
class ParticleCloud : public ICloud {
public:
    // Constructor/Destructor
    ParticleCloud() = default;
    ~ParticleCloud() override = default;

    // Implement methods from ICloud
    void connect() override;
    void disconnect(const ICloudDisconnectOptions& options) override;
    bool connected() override;
    bool syncTime() override;
    bool syncTimePending() override;
    bool syncTimeDone() override;
    bool process() override;
};

#endif // PARTICLE_CLOUD_H