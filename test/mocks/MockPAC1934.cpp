#include "MockPAC1934.h"

// Define fake functions
DEFINE_FAKE_VALUE_FUNC(bool, PAC1934_begin);
DEFINE_FAKE_VALUE_FUNC(float, PAC1934_readVoltage, uint8_t);
DEFINE_FAKE_VALUE_FUNC(float, PAC1934_readCurrent, uint8_t);
DEFINE_FAKE_VALUE_FUNC(float, PAC1934_readPower, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PAC1934_enableChannel, uint8_t, bool);

// Set default behavior
void setup_mock_PAC1934_defaults() {
    PAC1934_begin_fake.return_val = true;
    PAC1934_readVoltage_fake.return_val = 3.3f;
    PAC1934_readCurrent_fake.return_val = 0.1f;
    PAC1934_readPower_fake.return_val = 0.33f;
    PAC1934_enableChannel_fake.return_val = true;
}
