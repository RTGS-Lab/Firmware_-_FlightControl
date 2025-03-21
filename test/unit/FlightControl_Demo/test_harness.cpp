// Include necessary headers for the test harness
#include "test_harness.h"

// Include the actual source file to access its functions
#include "FlightControl_Demo.cpp"

namespace test_harness {
    // Implement the functions that call the private functions in FlightControl_Demo.cpp
    String callGetDataString() {
        return getDataString();
    }
    
    String callGetErrorString() {
        return getErrorString();
    }
    
    // Additional wrapper functions...
}