#include "Arduino.h"

// Global variables for time tracking
unsigned long _millis = 0;
unsigned long _micros = 0;

// Implementation of Arduino functions
unsigned long millis() {
    _millis += 100; // Advance time by 100ms each call for testing
    return _millis;
}

unsigned long micros() {
    _micros += 1000; // Advance time by 1000us each call for testing
    return _micros;
}

void delay(unsigned long ms) {
    // No-op in test
}

void delayMicroseconds(unsigned int us) {
    // No-op in test
}

void pinMode(uint8_t pin, uint8_t mode) {
    // No-op in test
}

void digitalWrite(uint8_t pin, uint8_t val) {
    // No-op in test
}

int digitalRead(uint8_t pin) {
    return LOW; // Default to LOW
}

int analogRead(uint8_t pin) {
    return 0; // Default to 0
}

void analogWrite(uint8_t pin, int val) {
    // No-op in test
}

int analogReadResolution(int bits) {
    return bits;
}

void analogWriteResolution(int bits) {
    // No-op in test
}

unsigned long pulseIn(uint8_t pin, uint8_t value, unsigned long timeout) {
    return 0; // Default to 0
}

long random(long min, long max) {
    return min;
}

long random(long max) {
    return 0;
}

void randomSeed(unsigned long seed) {
    // No-op in test
}

// Global instances
TwoWire Wire;
SPIClass SPI;