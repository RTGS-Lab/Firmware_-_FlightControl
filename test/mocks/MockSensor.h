#pragma once

#include <cstdint>
#include "MockParticle.h"

// This is a minimal mock of the Sensor base class that Kestrel inherits from
class Sensor {
public:
    Sensor() {}
    virtual ~Sensor() {}
    
    // Common base class properties
    static constexpr int MAX_NUM_ERRORS = 10; // Maximum number of errors to log
    uint32_t errors[MAX_NUM_ERRORS] = {0};
    uint8_t numErrors = 0;
    bool errorOverwrite = false;
    
    // Common methods
    virtual int throwError(uint32_t error) {
        // Simple error handling implementation
        if (numErrors < MAX_NUM_ERRORS) {
            errors[numErrors++] = error;
        } else {
            errors[MAX_NUM_ERRORS - 1] = error;
            errorOverwrite = true;
        }
        return 0;
    }
    
    // Virtual methods that would be implemented by derived classes
    virtual String begin(bool &criticalFault, bool &fault) { return ""; }
    virtual String getData(void) { return ""; }
    virtual String getMetadata(void) { return ""; }
    virtual String getErrors(void) { return ""; }
    virtual String selfDiagnostic(uint8_t diagnosticLevel) { return ""; }
};