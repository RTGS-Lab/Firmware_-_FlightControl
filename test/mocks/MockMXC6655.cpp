#include "MockMXC6655.h"

// Define fake functions for MXC6655
DEFINE_FAKE_VALUE_FUNC(bool, MXC6655_begin);
DEFINE_FAKE_VALUE_FUNC(bool, MXC6655_readAcceleration, float*, float*, float*);
DEFINE_FAKE_VALUE_FUNC(bool, MXC6655_resetOrientation);