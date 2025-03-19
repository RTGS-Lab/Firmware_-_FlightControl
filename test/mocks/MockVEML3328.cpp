#include "MockVEML3328.h"

// Define fake functions
DEFINE_FAKE_VALUE_FUNC(bool, VEML3328_begin);
DEFINE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readRed);
DEFINE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readGreen);
DEFINE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readBlue);
DEFINE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readClear);
DEFINE_FAKE_VALUE_FUNC(uint16_t, VEML3328_readIR);

// Set default behavior
void setup_mock_VEML3328_defaults() {
    VEML3328_begin_fake.return_val = true;
    VEML3328_readRed_fake.return_val = 500;
    VEML3328_readGreen_fake.return_val = 600;
    VEML3328_readBlue_fake.return_val = 450;
    VEML3328_readClear_fake.return_val = 800;
    VEML3328_readIR_fake.return_val = 300;
}
