/**
 * @file MockBMA456.h
 * @brief Mock implementation of IAccelerometer for testing
 *
 * Provides a mock implementation of the IAccelerometer interface
 * that can be used for unit testing without hardware dependencies.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#ifndef MOCK_BMA456_H
#define MOCK_BMA456_H

#include <gmock/gmock.h>
#include "IAccelerometer.h"

/**
 * @brief Google Mock implementation of IAccelerometer for testing
 */
class MockBMA456 : public IAccelerometer {
public:
    // Default constructor and destructor
    MockBMA456() = default;
    ~MockBMA456() override = default;

    // Mock methods
    MOCK_METHOD(int, begin, (), (override));
    MOCK_METHOD(float, getAccel, (uint8_t axis, uint8_t range), (override));
    MOCK_METHOD(int, updateAccelAll, (), (override));
    MOCK_METHOD(float, getTemp, (), (override));
    MOCK_METHOD(float*, getData, (), (override));
    MOCK_METHOD(float*, getOffset, (), (override));
    MOCK_METHOD(void, setOffset, (float offsetX, float offsetY, float offsetZ), (override));
};

#endif // MOCK_BMA456_H