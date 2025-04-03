#include "ParticleCloud.h"

void ParticleCloud::connect() {Particle.connect();}
void ParticleCloud::disconnect(const ICloudDisconnectOptions& options) {
    Particle.disconnect(CloudDisconnectOptions().graceful(options.graceful).timeout(options.timeout));
}
bool ParticleCloud::connected() {return Particle.connected();}
bool ParticleCloud::syncTime() {return Particle.syncTime();}
bool ParticleCloud::syncTimePending() {return Particle.syncTimePending();}
bool ParticleCloud::syncTimeDone() {return Particle.syncTimeDone();}
bool ParticleCloud::process() {return Particle.process();}