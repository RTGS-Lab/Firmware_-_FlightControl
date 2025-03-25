#pragma once

#include <cstdint>
#include <cstddef>  // For size_t

// SPI settings
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPI_CLOCK_DIV2 0x00
#define SPI_CLOCK_DIV4 0x01
#define SPI_CLOCK_DIV8 0x02
#define SPI_CLOCK_DIV16 0x03
#define SPI_CLOCK_DIV32 0x04
#define SPI_CLOCK_DIV64 0x05
#define SPI_CLOCK_DIV128 0x06

// Simple mock for the SPI class
class SPIClass {
public:
    void begin() {}
    void beginTransaction(uint32_t settings) {}
    void endTransaction() {}
    uint8_t transfer(uint8_t data) { return 0; }
    void transfer(void *buf, size_t count) {}
};

extern SPIClass SPI;