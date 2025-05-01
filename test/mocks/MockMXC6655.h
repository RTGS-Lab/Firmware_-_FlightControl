/**
 * @file MockAccelerometer.h
 * @brief Mock implementation of IAccelerometer for testing
 *
 * Provides a mock accelerometer implementation for unit testing
 * without requiring hardware.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef MOCK_MXC6655_H
 #define MOCK_MXC6655_H
 
 #include "IAccelerometer.h"
 #include <gmock/gmock.h>
 
 /**
  * @brief Mock implementation of IAccelerometer for testing
  */
 class MockMXC6655 : public IAccelerometer {
 public:
     MOCK_METHOD(int, begin, (), (override));
     MOCK_METHOD(float, getAccel, (uint8_t axis, uint8_t range), (override));
     MOCK_METHOD(int, updateAccelAll, (), (override));
     MOCK_METHOD(float, getTemp, (), (override));
     MOCK_METHOD(float*, getData, (), (override));
     MOCK_METHOD(float*, getOffset, (), (override));
     MOCK_METHOD(void, setOffset, (float offsetX, float offsetY, float offsetZ), (override));
 };
 
 #endif // MOCK_MXC6655_H