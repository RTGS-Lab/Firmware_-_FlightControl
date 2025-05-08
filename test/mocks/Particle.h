#ifndef MOCK_PARTICLE_H
#define MOCK_PARTICLE_H

// This file provides a minimal mock implementation of Particle's String class
// for testing purposes, without the need for actual Particle.h

#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm> // For std::min and std::max
#include <cmath>     // For isnan
#include <chrono>    // For time literals

using namespace std::chrono_literals; // For 20min, 30s literals

#define D2 2
#define D8 8
#define D22 22
#define A3 3
#define D7 7
#define A2 2
#define D6 6
#define A1 1
#define D5 5
#define D23 23
#define A6 6

// Hex formatting
#define HEX (unsigned char)16
// Make min and isnan available in global namespace for compatibility
using std::min;
using std::isnan;

// Time change event values
#define time_changed_sync 1
#define time_changed_manually 2

#define PLATFORM_BSOM 0
#define PLATFORM_B5SOM 1
#define PLATFORM_ID 0 

// Forward declaration
class StringSumHelper;

/**
 * Mock implementation of Particle's String class for testing
 * 
 * This class implements the minimum functionality needed for testing Li710
 * and can be expanded as needed for other components.
 */
class String {
public:
    // Constructors
    String(const char *cstr = "") {
        if (cstr) {
            _buffer = strdup(cstr);
            if (_buffer) {
                _length = strlen(cstr);
                _capacity = _length;
            }
        }
    }
    
    String(const String &str) : String(str._buffer) {}
    
    explicit String(char c) {
        _buffer = (char*)malloc(2);
        if (_buffer) {
            _buffer[0] = c;
            _buffer[1] = 0;
            _length = 1;
            _capacity = 1;
        }
    }
    
    explicit String(int value, unsigned char base = 10) {
        char buf[33];
        snprintf(buf, sizeof(buf), base == 10 ? "%d" : "%x", value);
        *this = String(buf);
    }
    
    // Make these unambiguous by ensuring the second parameter type is always distinct
    explicit String(float value, int decimalPlaces = 2) {
        char buf[33];
        snprintf(buf, sizeof(buf), "%.*f", decimalPlaces, value);
        *this = String(buf);
    }
    
    // Add constructor for double
    explicit String(double value, int decimalPlaces = 2) {
        char buf[33];
        snprintf(buf, sizeof(buf), "%.*f", decimalPlaces, value);
        *this = String(buf);
    }
    
    // Add constructor for unsigned long
    explicit String(unsigned long value, unsigned char base = 10) {
        char buf[33];
        if (base == 16) {
            snprintf(buf, sizeof(buf), "%lx", value);
        } else {
            snprintf(buf, sizeof(buf), "%lu", value);
        }
        *this = String(buf);
    }
    
    // Add constructor for long
    explicit String(long value, unsigned char base = 10) {
        char buf[33];
        if (base == 16) {
            snprintf(buf, sizeof(buf), "%lx", value);
        } else {
            snprintf(buf, sizeof(buf), "%ld", value);
        }
        *this = String(buf);
    }
    
    // Add constructor for uint32_t explicitly
    explicit String(uint32_t value, unsigned char base = 10) {
        char buf[33];
        if (base == 16) {
            snprintf(buf, sizeof(buf), "%x", value);
        } else {
            snprintf(buf, sizeof(buf), "%u", value);
        }
        *this = String(buf);
    }
    
    ~String() {
        if (_buffer) free(_buffer);
    }
    
    // Operators
    String& operator = (const String &rhs) {
        if (this == &rhs) return *this;
        if (_buffer) free(_buffer);
        if (rhs._buffer) {
            _buffer = strdup(rhs._buffer);
            _length = rhs._length;
            _capacity = rhs._capacity;
        } else {
            _buffer = nullptr;
            _length = 0;
            _capacity = 0;
        }
        return *this;
    }
    
    String& operator = (const char *cstr) {
        if (_buffer) free(_buffer);
        if (cstr) {
            _buffer = strdup(cstr);
            _length = strlen(cstr);
            _capacity = _length;
        } else {
            _buffer = nullptr;
            _length = 0;
            _capacity = 0;
        }
        return *this;
    }
    
    // Concatenation
    unsigned char concat(const String &str) {
        return concat(str._buffer, str._length);
    }
    
    unsigned char concat(const char *cstr) {
        if (!cstr) return 0;
        return concat(cstr, strlen(cstr));
    }
    
    unsigned char concat(const char *cstr, unsigned int length) {
        if (!cstr || !length) return 0;
        
        unsigned int newLen = _length + length;
        char *newBuffer = (char*)realloc(_buffer, newLen + 1);
        if (!newBuffer) return 0;
        
        _buffer = newBuffer;
        memcpy(_buffer + _length, cstr, length);
        _buffer[newLen] = 0;
        _length = newLen;
        _capacity = newLen;
        return 1;
    }
    
    unsigned char concat(char c) {
        char buf[2];
        buf[0] = c;
        buf[1] = 0;
        return concat(buf, 1);
    }
    
    unsigned char concat(int num) {
        char buf[12];
        snprintf(buf, sizeof(buf), "%d", num);
        return concat(buf, strlen(buf));
    }
    
    unsigned char concat(float num, int decimalPlaces = 2) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%.*f", decimalPlaces, num);
        return concat(buf, strlen(buf));
    }
    
    String& operator += (const String &rhs) {
        concat(rhs);
        return *this;
    }
    
    String& operator += (const char *cstr) {
        concat(cstr);
        return *this;
    }
    
    String& operator += (char c) {
        concat(c);
        return *this;
    }
    
    String& operator += (int num) {
        concat(num);
        return *this;
    }
    
    // Comparison
    unsigned char equals(const String &s) const {
        if (_length != s._length) return 0;
        if (!_buffer || !s._buffer) return (_buffer == s._buffer);
        return (strcmp(_buffer, s._buffer) == 0);
    }
    
    unsigned char equals(const char *cstr) const {
        if (!_buffer || !cstr) return (_buffer == cstr);
        return (strcmp(_buffer, cstr) == 0);
    }
    
    unsigned char operator == (const String &rhs) const {
        return equals(rhs);
    }
    
    unsigned char operator == (const char *cstr) const {
        return equals(cstr);
    }
    
    unsigned char operator != (const String &rhs) const {
        return !equals(rhs);
    }
    
    unsigned char operator != (const char *cstr) const {
        return !equals(cstr);
    }
    
    // Access
    char charAt(unsigned int index) const {
        if (index >= _length || !_buffer) return 0;
        return _buffer[index];
    }
    
    void setCharAt(unsigned int index, char c) {
        if (index >= _length || !_buffer) return;
        _buffer[index] = c;
    }
    
    char operator [] (unsigned int index) const {
        return charAt(index);
    }
    
    char& operator [] (unsigned int index) {
        static char dummy_writable_char;
        if (index >= _length || !_buffer) {
            dummy_writable_char = 0;
            return dummy_writable_char;
        }
        return _buffer[index];
    }
    
    const char* c_str() const {
        return _buffer ? _buffer : "";
    }
    
    operator const char*() const {
        return c_str();
    }
    
    unsigned int length() const {
        return _length;
    }
    
    // Search
    int indexOf(char ch) const {
        return indexOf(ch, 0);
    }
    
    int indexOf(char ch, unsigned int fromIndex) const {
        if (!_buffer || fromIndex >= _length) return -1;
        const char* temp = strchr(_buffer + fromIndex, ch);
        if (temp == nullptr) return -1;
        return temp - _buffer;
    }
    
    int indexOf(const String &str) const {
        return indexOf(str, 0);
    }
    
    int indexOf(const String &str, unsigned int fromIndex) const {
        if (!_buffer || !str._buffer || fromIndex >= _length) return -1;
        const char* found = strstr(_buffer + fromIndex, str._buffer);
        if (found == nullptr) return -1;
        return found - _buffer;
    }
<<<<<<< HEAD
    
    // Add endsWith method
    bool endsWith(const String &suffix) const {
        if (!_buffer || !suffix._buffer) return false;
        if (suffix._length > _length) return false;
        
        return (strcmp(_buffer + (_length - suffix._length), suffix._buffer) == 0);
    }
    
    bool endsWith(const char *suffix) const {
        if (!_buffer || !suffix) return false;
        size_t suffix_len = strlen(suffix);
        if (suffix_len > _length) return false;
        
        return (strcmp(_buffer + (_length - suffix_len), suffix) == 0);
    }
=======
>>>>>>> master

    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const {
        if (buf == nullptr || bufsize == 0) {
            return; // Nothing to do
        }

        // Always null-terminate the start of the buffer
        buf[0] = '\0';

        // Check if the source buffer is valid and index is within bounds
        if (_buffer == nullptr || index >= _length) {
            return; // Nothing to copy
        }

        // How many characters are available in the source string from index?
        size_t source_available = _length - index;

        // How many characters can we actually copy? (Need bufsize - 1 for the null terminator)
        size_t copy_len = std::min(source_available, (size_t)(bufsize - 1));

        // Perform the copy from the internal buffer
        if (copy_len > 0) { // Only copy if there's something to copy
            memcpy(buf, _buffer + index, copy_len);
        }

        // Add the null terminator at the end of the copied content
        buf[copy_len] = '\0';
    }
    
    // Modification
    String substring(unsigned int beginIndex) const {
        if (!_buffer || beginIndex >= _length) return String();
        return String(_buffer + beginIndex);
    }
    
    String substring(unsigned int beginIndex, unsigned int endIndex) const {
        if (!_buffer || beginIndex >= _length || endIndex <= beginIndex) return String();
        if (endIndex > _length) endIndex = _length;
        char temp[endIndex - beginIndex + 1];
        strncpy(temp, _buffer + beginIndex, endIndex - beginIndex);
        temp[endIndex - beginIndex] = 0;
        return String(temp);
    }
    
    String& remove(unsigned int index) {
        if (!_buffer || index >= _length) return *this;
        memmove(_buffer + index, _buffer + index + 1, _length - index);
        _length--;
        _buffer[_length] = 0;
        return *this;
    }
    
    String& remove(unsigned int index, unsigned int count) {
        if (!_buffer || index >= _length) return *this;
        if (count > _length - index) count = _length - index;
        memmove(_buffer + index, _buffer + index + count, _length - index - count + 1);
        _length -= count;
        return *this;
    }
    
    String& replace(char find, char replace) {
        if (!_buffer) return *this;
        for (unsigned int i = 0; i < _length; i++) {
            if (_buffer[i] == find) _buffer[i] = replace;
        }
        return *this;
    }
    
    String& trim() {
        if (!_buffer || !_length) return *this;
        
        // Find start of non-whitespace
        char *begin = _buffer;
        while (isspace(*begin)) begin++;
        
        // Find end of non-whitespace
        char *end = _buffer + _length - 1;
        while (end > begin && isspace(*end)) end--;
        
        // Calculate new length
        unsigned int newlen = end - begin + 1;
        
        // Move string if needed
        if (begin > _buffer) {
            memmove(_buffer, begin, newlen);
        }
        
        // Add null terminator
        _buffer[newlen] = 0;
        _length = newlen;
        
        return *this;
    }
    
    // Parsing
    long toInt() const {
        if (!_buffer) return 0;
        return atol(_buffer);
    }
    
    float toFloat() const {
        if (!_buffer) return 0;
        return atof(_buffer);
    }
    
private:
    char* _buffer = nullptr;
    unsigned int _length = 0;
    unsigned int _capacity = 0;
};

// String helper class for concatenation operations
class StringSumHelper : public String {
public:
    StringSumHelper(const String &s) : String(s) {}
    StringSumHelper(const char *p) : String(p) {}
    StringSumHelper(char c) : String(c) {}
    StringSumHelper(int num) : String(num) {}
    StringSumHelper(float num) : String(num) {}
};

// Concatenation operators
inline StringSumHelper operator + (const String &lhs, const String &rhs) {
    StringSumHelper result(lhs);
    result += rhs;
    return result;
}

inline StringSumHelper operator + (const String &lhs, const char *cstr) {
    StringSumHelper result(lhs);
    result += cstr;
    return result;
}

inline StringSumHelper operator + (const String &lhs, char c) {
    StringSumHelper result(lhs);
    result += c;
    return result;
}

inline StringSumHelper operator + (const String &lhs, int num) {
    StringSumHelper result(lhs);
    result += num;
    return result;
}

<<<<<<< HEAD
// Mock EEPROM class
class EEPROMClass {
public:
    template<typename T>
    void get(int address, T& value) {
        // Simple mock implementation - just provide default values
        value = T();
    }
    
    template<typename T>
    void put(int address, const T& value) {
        // Mock implementation - doesn't actually store anything
    }
};

// Global EEPROM instance - use inline to avoid multiple definition errors
inline EEPROMClass EEPROM;

// Mock RGB LED control
class RGBClass {
public:
    void control(bool controlEnabled) {
        // Mock implementation - doesn't do anything
    }
    
    void color(int r, int g, int b) {
        // Mock implementation - doesn't do anything
    }
};

// Global RGB instance - use inline to avoid multiple definition errors
inline RGBClass RGB;

// Mock HAL functions
inline uint32_t HAL_RNG_GetRandomNumber() {
    return rand(); // Simple mock implementation
}

// Mock functions for floating point control register
inline int __get_FPSCR() {
    return 0; // Mock implementation
}

inline void __set_FPSCR(int value) {
    // Mock implementation - doesn't do anything
}

#endif // MOCK_PARTICLE_H
=======
#endif // MOCK_PARTICLE_H
>>>>>>> master
