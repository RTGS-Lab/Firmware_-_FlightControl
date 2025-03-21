// test_harness.h
#pragma once

namespace test_harness {
    String callGetDataString();
    String callGetErrorString();
    String callGetDiagnosticString(uint8_t level);
    void callLogEvents(uint8_t type, uint8_t destination);
    // Add more as needed
}