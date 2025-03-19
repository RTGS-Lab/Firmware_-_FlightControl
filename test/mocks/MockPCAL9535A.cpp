#include "MockPCAL9535A.h"

// Define fake functions
DEFINE_FAKE_VALUE_FUNC(bool, PCAL9535A_begin);
DEFINE_FAKE_VALUE_FUNC(bool, PCAL9535A_pinMode, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCAL9535A_digitalWrite, uint8_t, uint8_t);
DEFINE_FAKE_VALUE_FUNC(uint8_t, PCAL9535A_digitalRead, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, PCAL9535A_begin_address, uint8_t);

// Set default behavior
void setup_mock_PCAL9535A_defaults() {
    PCAL9535A_begin_fake.return_val = true;
    PCAL9535A_pinMode_fake.return_val = true;
    PCAL9535A_digitalWrite_fake.return_val = true;
    PCAL9535A_digitalRead_fake.return_val = 0;
    PCAL9535A_begin_address_fake.return_val = true;
}