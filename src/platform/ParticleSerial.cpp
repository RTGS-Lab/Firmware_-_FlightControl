/**
 * @file    ParticleSerial.cpp
 * @brief   Particle implementation of ISerial interface
 */

#include "ParticleSerial.h"
#include "Particle.h"

// ParticleUSBSerial implementation (Serial)

void ParticleUSBSerial::begin(long speed) {
    Serial.begin(speed);
}

size_t ParticleUSBSerial::print(const char* str) {
    return Serial.print(str);
}

size_t ParticleUSBSerial::print(int value) {
    return Serial.print(value);
}

size_t ParticleUSBSerial::print(uint32_t value) {
    return Serial.print(value);
}

size_t ParticleUSBSerial::print(time_t value) {
    return Serial.print(value);
}

size_t ParticleUSBSerial::print(unsigned int value, int base) {
    return Serial.print(value, base);
}

size_t ParticleUSBSerial::print(float value) {
    return Serial.print(value);
}

size_t ParticleUSBSerial::print(double value) {
    return Serial.print(value);
}

size_t ParticleUSBSerial::println() {
    return Serial.println();
}

size_t ParticleUSBSerial::println(const char* str) {
    return Serial.println(str);
}

size_t ParticleUSBSerial::println(int value) {
    return Serial.println(value);
}

size_t ParticleUSBSerial::println(uint32_t value) {
    return Serial.println(value);
}

size_t ParticleUSBSerial::println(time_t value) {
    return Serial.println(value);
}

size_t ParticleUSBSerial::println(unsigned int value, int base) {
    return Serial.println(value, base);
}

void ParticleUSBSerial::flush() {
    Serial.flush();
}

// ParticleHardwareSerial implementation (Serial1)

void ParticleHardwareSerial::begin(unsigned long speed, uint32_t config) {
    Serial1.begin(speed, config);
}

size_t ParticleHardwareSerial::print(const char* str) {
    return Serial1.print(str);
}

size_t ParticleHardwareSerial::print(int value) {
    return Serial1.print(value);
}

size_t ParticleHardwareSerial::print(uint32_t value) {
    return Serial1.print(value);
}

size_t ParticleHardwareSerial::print(time_t value) {
    return Serial1.print(value);
}

size_t ParticleHardwareSerial::print(unsigned int value, int base) {
    return Serial1.print(value, base);
}

size_t ParticleHardwareSerial::print(float value) {
    return Serial1.print(value);
}

size_t ParticleHardwareSerial::print(double value) {
    return Serial1.print(value);
}

size_t ParticleHardwareSerial::println() {
    return Serial1.println();
}

size_t ParticleHardwareSerial::println(const char* str) {
    return Serial1.println(str);
}

size_t ParticleHardwareSerial::println(int value) {
    return Serial1.println(value);
}

size_t ParticleHardwareSerial::println(uint32_t value) {
    return Serial1.println(value);
}

size_t ParticleHardwareSerial::println(time_t value) {
    return Serial1.println(value);
}

size_t ParticleHardwareSerial::println(unsigned int value, int base) {
    return Serial1.println(value, base);
}

void ParticleHardwareSerial::flush() {
    Serial1.flush();
}