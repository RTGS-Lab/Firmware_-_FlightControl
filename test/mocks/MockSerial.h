#ifndef MOCK_SERIAL_H
#define MOCK_SERIAL_H

#include <gmock/gmock.h>
#include "ISerial.h" // Include the interface definition

/**
 * @brief Google Mock implementation of ISerial for testing.
 */
class MockSerial : public ISerial {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(void, begin, (long speed), (override));
    MOCK_METHOD(void, begin, (unsigned long speed, uint32_t config), (override));
    MOCK_METHOD(size_t, print, (const char* str), (override));
    MOCK_METHOD(size_t, print, (int value), (override));
    MOCK_METHOD(size_t, print, (uint32_t value), (override));
    MOCK_METHOD(size_t, print, (time_t value), (override));
    MOCK_METHOD(size_t, print, (unsigned int value, int base), (override));
    MOCK_METHOD(size_t, print, (float value), (override));
    MOCK_METHOD(size_t, print, (double value), (override));
    MOCK_METHOD(size_t, println, (), (override));
    MOCK_METHOD(size_t, println, (const char* str), (override));
    MOCK_METHOD(size_t, println, (int value), (override));
    MOCK_METHOD(size_t, println, (uint32_t value), (override));
    MOCK_METHOD(size_t, println, (time_t value), (override));
    MOCK_METHOD(size_t, println, (unsigned int value, int base), (override));
    MOCK_METHOD(void, flush, (), (override));
};

#endif // MOCK_SERIAL_H