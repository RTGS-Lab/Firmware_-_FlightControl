#include "FlightControl_DemoTestHarness.h"

// Declare the functions from the original file
extern String getDataString();
extern String getErrorString();
extern String getDiagnosticString(uint8_t level);
extern void logEvents(uint8_t type, uint8_t destination);

namespace FlightControl_DemoTestHarness {
    String callGetDataString() {
        return getDataString();
    }
    // Other functions
}