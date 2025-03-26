#include "Particle.h"

// Define global instances
SystemClass System;
TwoWire Wire;
MockSerialPort Serial;
MockSerialPort Serial1;

// Basic implementations of Arduino functions
unsigned long millis() {
    static unsigned long fake_millis = 0;
    fake_millis += 100; // Increment by 100ms each call
    return fake_millis;
}

void delay(unsigned long ms) {
    // No-op in test
}

void pinMode(uint16_t pin, uint8_t mode) {
    // No-op in test
}

void digitalWrite(uint16_t pin, uint8_t value) {
    // No-op in test
}

int digitalRead(uint16_t pin) {
    return 0; // Default to LOW
}