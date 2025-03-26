#ifndef ARDUINO_H_MOCK
#define ARDUINO_H_MOCK

// Include Particle.h first to make sure shared types come from there
#include "Particle.h"

// Basic Arduino types
typedef uint8_t byte;
typedef bool boolean;

// Arduino constants - only defined if not already defined
#ifndef HIGH
#define HIGH 0x1
#endif
#ifndef LOW
#define LOW  0x0
#endif
#ifndef INPUT
#define INPUT 0x0
#endif
#ifndef OUTPUT
#define OUTPUT 0x1
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 0x2
#endif

// SPI related constants
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV32 0x06
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03

// Wiring.h based functions
unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);
int analogReadResolution(int bits);
void analogWriteResolution(int bits);
unsigned long pulseIn(uint8_t pin, uint8_t value, unsigned long timeout = 1000000L);

// Random functions
long random(long min, long max);
long random(long max);
void randomSeed(unsigned long seed);

// External variables needed by Arduino libraries
extern unsigned long _millis;
extern unsigned long _micros;

// Use MockSerialPort from Particle.h for Serial and Serial1
// No need to redefine them here

// Use TwoWire from Particle.h - no need to redefine
// Wire object instance is defined in Particle.cpp

// SPI class
class SPIClass {
public:
    static const uint8_t MOSI = 11;
    static const uint8_t MISO = 12;
    static const uint8_t SCK = 13;
    static const uint8_t SS = 10;
    
    void begin() {}
    void end() {}
    void beginTransaction(uint8_t pin, uint8_t mode, uint32_t speed) {}
    void endTransaction() {}
    uint8_t transfer(uint8_t data) { return 0; }
    uint16_t transfer16(uint16_t data) { return 0; }
    void transfer(void *buf, size_t count) {}
    void setBitOrder(uint8_t bitOrder) {}
    void setDataMode(uint8_t dataMode) {}
    void setClockDivider(uint8_t clockDiv) {}
};

// SPI object instance
extern SPIClass SPI;

#endif // ARDUINO_H_MOCK