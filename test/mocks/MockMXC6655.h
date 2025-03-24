#pragma once

#include "fff.h"
#include <cstdint>

// Declare fake functions for MXC6655 accelerometer
DECLARE_FAKE_VALUE_FUNC(bool, MXC6655_begin);
DECLARE_FAKE_VALUE_FUNC(bool, MXC6655_readAcceleration, float*, float*, float*);
DECLARE_FAKE_VALUE_FUNC(bool, MXC6655_resetOrientation);

// Mock class for MXC6655 accelerometer
class MXC6655 {
public:
    // Default mock values
    float x_acceleration = 0.0f;
    float y_acceleration = 0.0f;
    float z_acceleration = 1.0f; // Default to 1G in Z direction (earth gravity)
    
    bool begin() { return MXC6655_begin(); }
    
    bool readAcceleration(float* x, float* y, float* z) { 
        *x = x_acceleration;
        *y = y_acceleration;
        *z = z_acceleration;
        return MXC6655_readAcceleration(x, y, z); 
    }
    
    bool resetOrientation() { return MXC6655_resetOrientation(); }
};