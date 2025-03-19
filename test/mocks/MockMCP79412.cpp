#include "MockMCP79412.h"

// Define fake functions
DEFINE_FAKE_VALUE_FUNC(bool, MCP79412_begin);
DEFINE_FAKE_VALUE_FUNC(time_t, MCP79412_getTime);
DEFINE_FAKE_VALUE_FUNC(bool, MCP79412_setTime, time_t);

// Set default behavior
void setup_mock_MCP79412_defaults() {
    MCP79412_begin_fake.return_val = true;
    MCP79412_getTime_fake.return_val = time(NULL); // Current time
    MCP79412_setTime_fake.return_val = true;
}