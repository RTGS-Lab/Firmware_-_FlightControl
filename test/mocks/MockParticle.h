#ifndef MOCK_PARTICLE_H
#define MOCK_PARTICLE_H

#include <gmock/gmock.h>
#include <string>

// Define system event type similar to Particle's definition
typedef int system_event_t;

// Event type values 
const int time_changed = 1;
const int out_of_memory = 2;

// Mock String class for tests
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

private:
    std::string value_;
};

// For gtest to print Strings in a readable way
inline std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << str.c_str();
}

// Mock for Particle System functionality
class MockSystemClass {
public:
    MOCK_METHOD(void, on, (int event, void (*handler)(system_event_t, int)));
    MOCK_METHOD(uint32_t, resetReason, ());
    MOCK_METHOD(bool, connected, ());
};

// Mock for Wire/I2C functionality
class MockTwoWire {
public:
    MOCK_METHOD(bool, isEnabled, ());
    MOCK_METHOD(void, begin, ());
    MOCK_METHOD(void, setClock, (uint32_t));
    MOCK_METHOD(void, beginTransmission, (uint8_t));
    MOCK_METHOD(uint8_t, endTransmission, (bool));
    MOCK_METHOD(uint8_t, requestFrom, (uint8_t, uint8_t, bool));
    MOCK_METHOD(int, available, ());
    MOCK_METHOD(int, read, ());
    MOCK_METHOD(size_t, write, (uint8_t));
    MOCK_METHOD(size_t, write, (const uint8_t*, size_t));
};

// Mock EEPROM namespace
namespace EEPROM {
    template<typename T>
    void get(int addr, T& val) {
        val = T(); // Default construct the value
    }
}

// Define platform IDs 
#define PLATFORM_BSOM 23
#define PLATFORM_B5SOM 32
#define PLATFORM_ID PLATFORM_BSOM

#endif // MOCK_PARTICLE_H