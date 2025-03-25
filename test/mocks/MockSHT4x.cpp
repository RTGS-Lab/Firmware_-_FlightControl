#include "MockSHT4x.h"

// Define fake functions
DEFINE_FAKE_VALUE_FUNC(bool, SHT4x_begin);
DEFINE_FAKE_VALUE_FUNC(bool, SHT4x_setPrecision, uint8_t);
DEFINE_FAKE_VALUE_FUNC(bool, SHT4x_readTemperature);
DEFINE_FAKE_VALUE_FUNC(bool, SHT4x_readHumidity);
DEFINE_FAKE_VALUE_FUNC(bool, SHT4x_getEvent);

// Set default behavior
void setup_mock_SHT4x_defaults() {
    SHT4x_begin_fake.return_val = true;
    SHT4x_setPrecision_fake.return_val = true;
    SHT4x_readTemperature_fake.return_val = true;
    SHT4x_readHumidity_fake.return_val = true;
    SHT4x_getEvent_fake.return_val = true;
}
