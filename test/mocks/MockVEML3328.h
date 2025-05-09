/**
 * @file MockVEML3328.h
 * @brief Mock implementation of IAmbientLight for testing.
 *
 * This class uses Google Mock to create a testable version of the ambient light sensor interface.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef MOCK_VEML3328_H
 #define MOCK_VEML3328_H
 
 #include <gmock/gmock.h>
 #include "IAmbientLight.h"
 
 /**
  * @brief Google Mock implementation of IAmbientLight for testing.
  */
 class MockVEML3328 : public IAmbientLight {
 public:
     // Mock all methods defined in the interface
     MOCK_METHOD(int, begin, (), (override));
     MOCK_METHOD(float, getValue, (Channel channel), (override));
     MOCK_METHOD(float, getValue, (Channel channel, bool &state), (override));
     MOCK_METHOD(float, getLux, (), (override));
     MOCK_METHOD(int, autoRange, (), (override));
 };
 
 #endif // MOCK_VEML3328_H