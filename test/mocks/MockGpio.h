#ifndef MOCK_GPIO_H
#define MOCK_GPIO_H

#include <gmock/gmock.h>
#include "IGpio.h" // Include the interface definition

/**
 * @brief Google Mock implementation of IGpio for testing.
 */
class MockGpio : public IGpio {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(void, pinMode, (uint16_t pin, IPinMode mode), (override));
    MOCK_METHOD(void, digitalWrite, (uint16_t pin, uint8_t value), (override));
    MOCK_METHOD(int32_t, digitalRead, (uint16_t pin), (override));
};

#endif // MOCK_GPIO_H