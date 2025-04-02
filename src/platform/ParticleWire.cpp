#include "ParticleWire.h"

void ParticleWire::begin(){Wire.begin();}
void ParticleWire::setClock(uint32_t speed){Wire.setClock(speed);}
bool ParticleWire::isEnabled(){return Wire.isEnabled();}
void ParticleWire::beginTransmission(int value){Wire.beginTransmission(value);}
uint8_t ParticleWire::endTransmission(){return Wire.endTransmission();}
size_t ParticleWire::write(uint8_t value){return Wire.write(value);}
int ParticleWire::reset(){return Wire.reset();}

