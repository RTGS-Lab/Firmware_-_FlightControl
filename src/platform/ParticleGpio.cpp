#include "ParticleGpio.h"

void ParticleGpio::pinMode(uint16_t pin, IPinMode mode){
    PinMode particlePinMode;
    switch(mode)
    {
        case IPinMode::INPUT:
            particlePinMode = ::INPUT;
            break;
        case IPinMode::OUTPUT:
            particlePinMode = ::OUTPUT;
            break;
        default:
            particlePinMode = ::INPUT;
            break;
    }
    ::pinMode(pin, particlePinMode);
}
void ParticleGpio::digitalWrite(uint16_t pin, uint8_t value){::digitalWrite(pin, value);}
int32_t ParticleGpio::digitalRead(uint16_t pin){return ::digitalRead(pin);}