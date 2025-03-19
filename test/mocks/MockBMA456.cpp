#include "MockBMA456.h"

// Define fake functions for BMA456
DEFINE_FAKE_VALUE_FUNC(bool, BMA456_begin);
DEFINE_FAKE_VALUE_FUNC(bool, BMA456_readAcceleration, float*, float*, float*);
DEFINE_FAKE_VALUE_FUNC(bool, BMA456_enableStepCounter, bool);
DEFINE_FAKE_VALUE_FUNC(uint32_t, BMA456_getStepCount);
DEFINE_FAKE_VALUE_FUNC(bool, BMA456_resetStepCounter);

// Initialize the global object
BMA456 accel_bma456;