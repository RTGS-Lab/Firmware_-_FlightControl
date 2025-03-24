#include "MockPCA9634.h"

// Define fake functions for PCA9634
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_begin);
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_begin_address, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_setLEDOutputMode, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_setLEDDriverMode, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_setBrightness, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCA9634_setOutputState, uint8_t, uint8_t);