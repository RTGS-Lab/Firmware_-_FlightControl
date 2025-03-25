#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions for BMA456 accelerometer
DECLARE_FAKE_VALUE_FUNC(bool, BMA456_begin);
DECLARE_FAKE_VALUE_FUNC(bool, BMA456_readAcceleration, float*, float*, float*);
DECLARE_FAKE_VALUE_FUNC(bool, BMA456_enableStepCounter, bool);
DECLARE_FAKE_VALUE_FUNC(uint32_t, BMA456_getStepCount);
DECLARE_FAKE_VALUE_FUNC(bool, BMA456_resetStepCounter);

// Mock class for BMA456 via arduino_bma456
class BMA456 {
public:
    // Default mock values
    float x_acceleration = 0.0f;
    float y_acceleration = 0.0f;
    float z_acceleration = 1.0f; // Default to 1G in Z direction (earth gravity)
    uint32_t step_count = 0;
    
    bool begin() { return BMA456_begin(); }
    
    bool readAcceleration(float* x, float* y, float* z) { 
        *x = x_acceleration;
        *y = y_acceleration;
        *z = z_acceleration;
        return BMA456_readAcceleration(x, y, z); 
    }
    
    bool enableStepCounter(bool enable) { return BMA456_enableStepCounter(enable); }
    uint32_t getStepCount() { return BMA456_getStepCount(); }
    bool resetStepCounter() { return BMA456_resetStepCounter(); }
};

// This simulates the global object provided by the arduino_bma456 library
extern BMA456 accel_bma456;