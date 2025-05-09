/**
 * @file    ParticleSerial.h
 * @brief   Particle implementation of ISerial interface
 */

#ifndef __PARTICLE_SERIAL_H
#define __PARTICLE_SERIAL_H

#include "../../lib/FlightControl-platform-dependencies/src/ISerial.h"

/**
 * @brief Particle Serial implementation for Serial (USB)
 */
class ParticleUSBSerial : public ISerial {
public:
    ParticleUSBSerial() = default;
    virtual ~ParticleUSBSerial() = default;

    void begin(long speed) override;
    void begin(unsigned long speed, uint32_t config) override { begin((long)speed); }
    size_t print(const char* str) override;
    size_t print(int value) override;
    size_t print(uint32_t value) override;
    size_t print(time_t value) override;
    size_t print(unsigned int value, int base = 10) override;
    size_t print(float value) override;
    size_t print(double value) override;
    size_t println() override;
    size_t println(const char* str) override;
    size_t println(int value) override;
    size_t println(uint32_t value) override;
    size_t println(time_t value) override;
    size_t println(unsigned int value, int base = 10) override;
    void flush() override;
};

/**
 * @brief Particle Serial implementation for Serial1 (Hardware UART)
 */
class ParticleHardwareSerial : public ISerial {
public:
    ParticleHardwareSerial() = default;
    virtual ~ParticleHardwareSerial() = default;

    void begin(unsigned long speed, uint32_t config) override;
    void begin(long speed) override { begin((unsigned long)speed, 0); }
    size_t print(const char* str) override;
    size_t print(int value) override;
    size_t print(uint32_t value) override;
    size_t print(time_t value) override;
    size_t print(unsigned int value, int base = 10) override;
    size_t print(float value) override;
    size_t print(double value) override;
    size_t println() override;
    size_t println(const char* str) override;
    size_t println(int value) override;
    size_t println(uint32_t value) override;
    size_t println(time_t value) override;
    size_t println(unsigned int value, int base = 10) override;
    void flush() override;
};

#endif // __PARTICLE_SERIAL_H