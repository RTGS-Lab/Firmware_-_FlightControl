#pragma once

#include "MockParticle.h"
#include <cstdint>
#include <ctime>

// Mock Particle types and functions to satisfy dependencies
typedef uint8_t system_event_t;

// System event handlers
constexpr system_event_t TIME_CHANGED = 1;
constexpr system_event_t LOW_MEMORY = 2;

// Mock Particle functions
inline void System_on(system_event_t event, void (*handler)(system_event_t, int)) {
    // Just do nothing in mock
}

inline int Time_now() {
    return time(NULL);
}

inline bool Particle_connected() {
    return true;
}

inline bool Particle_syncTime() {
    return true;
}

// Define the actual Particle API namespace/functions
namespace Particle {
    inline bool connected() { return Particle_connected(); }
    inline bool syncTime() { return Particle_syncTime(); }
    
    namespace System {
        inline void on(system_event_t event, void (*handler)(system_event_t, int)) {
            System_on(event, handler);
        }
    }
    
    namespace Time {
        inline int now() { return Time_now(); }
    }
}