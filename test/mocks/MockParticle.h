// test/mocks/mock_particle.h
#pragma once
#include <string>
#include <cstring>
#include <cstdint>

// Basic String implementation to match Particle's String class
class String {
private:
    std::string data;

public:
    String() : data("") {}
    String(const char* str) : data(str ? str : "") {}
    String(const std::string& str) : data(str) {}
    String(int value) : data(std::to_string(value)) {}
    
    // Common methods used in the tests
    bool equals(const String& other) const { return data == other.data; }
    bool startsWith(const String& prefix) const { 
        return data.find(prefix.data) == 0; 
    }
    bool endsWith(const String& suffix) const {
        if (suffix.data.length() > data.length()) return false;
        return data.rfind(suffix.data) == (data.length() - suffix.data.length());
    }
    
    // Operators
    String operator+(const String& rhs) const {
        return String(data + rhs.data);
    }
    String& operator+=(const String& rhs) {
        data += rhs.data;
        return *this;
    }
    bool operator==(const String& rhs) const {
        return data == rhs.data;
    }
    
    // Conversion operators
    operator const char*() const { return data.c_str(); }
    
    // Common String methods
    size_t length() const { return data.length(); }
    const char* c_str() const { return data.c_str(); }
    
    // Specific methods needed for your FlightControl_Demo
    bool endsWith(char c) const {
        return !data.empty() && data.back() == c;
    }
    
    int lastIndexOf(char c) const {
        size_t pos = data.rfind(c);
        return pos != std::string::npos ? static_cast<int>(pos) : -1;
    }
    
    int lastIndexOf(const String& str) const {
        size_t pos = data.rfind(str.data);
        return pos != std::string::npos ? static_cast<int>(pos) : -1;
    }
    
    // Add more methods as they're needed
};

// Add other Particle types and functions as needed
// For example:
typedef uint8_t byte;

// Time functions
unsigned long millis() { return 0; }  // Mock implementation