#ifndef PARTICLE_H_MOCK
#define PARTICLE_H_MOCK

#include <cstdint>
#include <ctime>
#include <string>
#include <gmock/gmock.h>

// Must come before any GoogleTest stuff to avoid conflicts
#ifndef SERIAL_8N1
#define SERIAL_8N1 0x00
#endif

// Undefine any macros that might cause conflicts with library headers
#ifdef A
#undef A
#endif

// Define basic Particle types
typedef int system_event_t;

// Define common Particle event types
const int time_changed = 1;
const int out_of_memory = 2;

// String class compatible with Particle's
class String {
public:
    String() : value_("") {}
    String(const char* str) : value_(str ? str : "") {}
    String(const std::string& str) : value_(str) {}
    String(int val) : value_(std::to_string(val)) {}
    String(long val) : value_(std::to_string(val)) {}
    String(float val, int decimalPlaces = 2) : value_(std::to_string(val)) {}
    
    bool equals(const String& other) const {
        return value_ == other.value_;
    }

    String substring(size_t beginIndex) const {
        return value_.substr(beginIndex);
    }

    String substring(size_t beginIndex, size_t endIndex) const {
        return value_.substr(beginIndex, endIndex - beginIndex);
    }

    size_t length() const {
        return value_.length();
    }
    
    const char* c_str() const {
        return value_.c_str();
    }

    bool operator==(const String& other) const {
        return value_ == other.value_;
    }

    bool operator==(const char* other) const {
        return value_ == other;
    }

    String operator+(const String& other) const {
        return String(value_ + other.value_);
    }

    String operator+(const char* other) const {
        return String(value_ + other);
    }

    String operator+(int other) const {
        return String(value_ + std::to_string(other));
    }

    String endsWith(const String& suffix) const {
        if (suffix.length() > value_.length()) return false;
        return value_.compare(value_.length() - suffix.length(), suffix.length(), suffix.value_) == 0;
    }

    String remove(size_t pos, size_t len) {
        if (pos >= value_.length()) return *this;
        return String(value_.erase(pos, len));
    }

private:
    std::string value_;
};

// For gtest to print Strings in a readable way
inline std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << str.c_str();
}

// Mock minimal Arduino functions that Particle depends on
typedef uint8_t byte;
typedef bool boolean;

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define SERIAL_8N1 0x00
#define HEX 16

// Stream base class
class Stream {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual ~Stream() {}
};

// Print base class
class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) {
        size_t n = 0;
        for (size_t i = 0; i < size; i++) {
            n += write(buffer[i]);
        }
        return n;
    }
    virtual ~Print() {}
};

// Mock Serial objects with gmock that inherit from Stream for compatibility
class MockSerialPort : public Stream, public Print {
public:
    // For MockSerialPort
    MOCK_METHOD(void, begin, (unsigned long, uint8_t));
    MOCK_METHOD(void, print, (const char*));
    MOCK_METHOD(void, print, (int));
    MOCK_METHOD(void, print, (const String&));
    MOCK_METHOD(void, println, (const char*));
    MOCK_METHOD(void, println, (int));
    MOCK_METHOD(void, println, (const String&));
    MOCK_METHOD(void, println, ());
    
    // From Stream
    MOCK_METHOD(int, available, (), (override));
    MOCK_METHOD(int, read, (), (override));
    MOCK_METHOD(int, peek, (), (override));
    MOCK_METHOD(void, flush, (), (override));
    
    // From Print
    MOCK_METHOD(size_t, write, (uint8_t), (override));
};

extern MockSerialPort Serial;
extern MockSerialPort Serial1;

// Wire class for I2C
class TwoWire {
public:
    bool isEnabled() { return true; }
    void begin() {}
    void setClock(uint32_t clock) {}
    void beginTransmission(uint8_t address) {}
    uint8_t endTransmission(bool stopBit = true) { return 0; }
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stopBit = true) { return 0; }
    int available() { return 0; }
    int read() { return 0; }
    size_t write(uint8_t data) { return 1; }
    size_t write(const uint8_t* data, size_t len) { return len; }
};

extern TwoWire Wire;

// EEPROM
class EEPROMClass {
public:
    void get(int addr, float val) {}
};

// System class that appears in many Particle programs
class SystemClass {
public:
    void on(int event, void (*handler)(system_event_t, int)) {}
    uint32_t resetReason() { return 0; }
    bool connected() { return true; }
    void reset() {}
    const char* version() { return "1.0.0"; }
    String deviceID() { return "TEST_DEVICE_ID"; }
    bool freeMemory() { return true; }
};

extern SystemClass System;

// Define pin names
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19
#define D20 20
#define D21 21
#define D22 22
#define D23 23
#define A0 24
#define A1 25
#define A2 26
#define A3 27
#define A4 28
#define A5 29
#define A6 30
#define A7 31

// Platform IDs
#define PLATFORM_BSOM 23
#define PLATFORM_B5SOM 32
#define PLATFORM_ID PLATFORM_BSOM

// Arduino-like functions
unsigned long millis();
void delay(unsigned long ms);
void pinMode(uint16_t pin, uint8_t mode);
void digitalWrite(uint16_t pin, uint8_t value);
int digitalRead(uint16_t pin);

extern EEPROMClass EEPROM;

// Define min/max functions if not already defined
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#endif // PARTICLE_H_MOCK