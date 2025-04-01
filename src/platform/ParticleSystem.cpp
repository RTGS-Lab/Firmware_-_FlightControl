#include "ParticleSystem.h"
#include "Particle.h" // Make sure Particle.h is included for Particle types/functions

// --- Method Implementations ---

void ParticleSystem::on(IEventType eventType, SystemEventHandler handler) {
    system_event_t particleEvent;
    switch (eventType) {
        case IEventType::TIME_CHANGED:  particleEvent = time_changed; break;
        case IEventType::OUT_OF_MEMORY: particleEvent = out_of_memory; break;
        default: Log.error("Unsupported IEventType in ParticleSystem::on"); return;
    }

    // Wrapper lambda captures handler by value
    auto particleHandlerWrapper = [handler](system_event_t sys_event, int data, void*) {
        IEventType interfaceEventType;
        switch (sys_event) {
            case time_changed:  interfaceEventType = IEventType::TIME_CHANGED; break;
            case out_of_memory: interfaceEventType = IEventType::OUT_OF_MEMORY; break;
            default: Log.warn("Unexpected system_event_t in wrapper: %d", (int)sys_event); return;
        }
        if (handler) { handler(interfaceEventType, data); }
    };

    System.on(particleEvent, particleHandlerWrapper);
}

IResetReason ParticleSystem::resetReason() {
    int particleReason = System.resetReason();
    // Assuming Particle's RESET_REASON_* constants match the values used in IResetReason enum
    switch (particleReason) {
        // Keep only the cases defined by Particle's resetReason() return values
        case ::RESET_REASON_PIN_RESET:          return IResetReason::PIN_RESET;
        case ::RESET_REASON_POWER_MANAGEMENT:   return IResetReason::POWER_MANAGEMENT;
        case ::RESET_REASON_POWER_DOWN:         return IResetReason::POWER_DOWN;
        case ::RESET_REASON_POWER_BROWNOUT:     return IResetReason::POWER_BROWNOUT;
        case ::RESET_REASON_WATCHDOG:           return IResetReason::WATCHDOG;
        case ::RESET_REASON_UPDATE:             return IResetReason::UPDATE;
        case ::RESET_REASON_UPDATE_ERROR:       return IResetReason::UPDATE_ERROR;
        case ::RESET_REASON_UPDATE_TIMEOUT:     return IResetReason::UPDATE_TIMEOUT;
        case ::RESET_REASON_FACTORY_RESET:      return IResetReason::FACTORY_RESET;
        case ::RESET_REASON_SAFE_MODE:          return IResetReason::SAFE_MODE;
        case ::RESET_REASON_DFU_MODE:           return IResetReason::DFU_MODE;
        case ::RESET_REASON_PANIC:              return IResetReason::PANIC;
        case ::RESET_REASON_USER:               return IResetReason::USER;
        // Note: RESET_REASON_CONFIG_UPDATE might not be a standard Particle reason code. Check documentation.
        // If it's not standard, remove or map appropriately.
        // case ::RESET_REASON_CONFIG_UPDATE:      return IResetReason::CONFIG_UPDATE; // Remove if not standard
        case ::RESET_REASON_UNKNOWN: // Particle's unknown/default
        default:
            // Map Particle's 0 (NONE) or others to UNKNOWN
            return IResetReason::UNKNOWN;
    }
}

uint32_t ParticleSystem::freeMemory() {
    return System.freeMemory();
}

// *** Updated sleep implementation ***
IWakeupReason ParticleSystem::sleep(const ISleepConfig& iConfig) {
    // 1. Create the Particle-specific configuration object
    particle::SystemSleepConfiguration particleConfig;

    // 2. Translate from ISleepConfig (interface) to particleConfig (Particle)
    // Translate Mode
    switch(iConfig.mode) {
        case ISleepMode::ULTRA_LOW_POWER:
            particleConfig.mode(SystemSleepMode::ULTRA_LOW_POWER);
            break;
        case ISleepMode::STOP:
            particleConfig.mode(SystemSleepMode::STOP);
            break;
        case ISleepMode::HIBERNATE:
            particleConfig.mode(SystemSleepMode::HIBERNATE);
            break;
        // Add other ISleepMode cases -> SystemSleepMode mapping if needed
        default:
            Log.warn("Unsupported ISleepMode passed, defaulting to ULP");
            particleConfig.mode(SystemSleepMode::ULTRA_LOW_POWER); // Default?
            break;
    }

    // Translate Duration
    if (iConfig.duration.count() > 0) {
        particleConfig.duration(iConfig.duration);
    }

    // Translate Wake Pin
    // Assuming 0xFFFF was used as the "invalid/not set" marker in ISleepConfig
    if (iConfig.wakePin != 0xFFFF) {
         // Translate IInterruptMode (interface) to Particle's InterruptMode
         InterruptMode particleInterruptMode;
         switch(iConfig.wakePinMode) {
             case IInterruptMode::FALLING: particleInterruptMode = FALLING; break;
             case IInterruptMode::RISING:  particleInterruptMode = RISING; break;
             case IInterruptMode::CHANGE:  particleInterruptMode = CHANGE; break;
             case IInterruptMode::NONE:
             default:
                 Log.error("Unsupported IInterruptMode for wake pin %u, defaulting to CHANGE.", iConfig.wakePin);
                 particleInterruptMode = CHANGE; // Default?
                 break;
         }
         particleConfig.gpio(iConfig.wakePin, particleInterruptMode);
    }

    // Translate Network Interface setting
    switch(iConfig.network) {
        case INetworkInterfaceIndex::CELLULAR:
             particleConfig.network(NETWORK_INTERFACE_CELLULAR); // Basic keep alive
             // particleConfig.network(NETWORK_INTERFACE_CELLULAR, SystemSleepNetworkFlag::INACTIVE_STANDBY); // Alternative for lower power standby
             break;
        case INetworkInterfaceIndex::WIFI:
             particleConfig.network(NETWORK_INTERFACE_WIFI_AP);
             break;
        case INetworkInterfaceIndex::ETHERNET:
              particleConfig.network(NETWORK_INTERFACE_ETHERNET);
              break;
        // case INetworkInterfaceIndex::ALL: // Ambiguous, map carefully if needed
        //      particleConfig.network(NETWORK_INTERFACE_CELLULAR);
        //      particleConfig.network(NETWORK_INTERFACE_WIFI); // Example
        //      break;
        case INetworkInterfaceIndex::NONE:
        case INetworkInterfaceIndex::LOOPBACK: // Loopback doesn't make sense for sleep network config
        default:
             // Default is often network off, so potentially do nothing here,
             // or explicitly ensure it's off if Particle API requires it.
             break;
    }

    // Translate other flags from ISleepConfig if added...
    // e.g., if (iConfig.waitForCloud) { particleConfig.flag(SystemSleepFlag::WAIT_CLOUD); }


    // 3. Call the actual Particle function with the configured Particle object
    SystemSleepResult result = System.sleep(particleConfig);

    // 4. Translate Particle's SystemSleepWakeupReason back to our IWakeupReason
    switch (result.wakeupReason()) {
        // Map all defined reasons
        case SystemSleepWakeupReason::BY_GPIO:      return IWakeupReason::BY_GPIO;
        case SystemSleepWakeupReason::BY_ADC:       return IWakeupReason::BY_ADC;
        case SystemSleepWakeupReason::BY_DAC:       return IWakeupReason::BY_DAC;
        case SystemSleepWakeupReason::BY_RTC:       return IWakeupReason::BY_RTC;
        case SystemSleepWakeupReason::BY_LPCOMP:    return IWakeupReason::BY_LPCOMP;
        case SystemSleepWakeupReason::BY_USART:     return IWakeupReason::BY_USART;
        case SystemSleepWakeupReason::BY_I2C:       return IWakeupReason::BY_I2C;
        case SystemSleepWakeupReason::BY_SPI:       return IWakeupReason::BY_SPI;
        case SystemSleepWakeupReason::BY_TIMER:     return IWakeupReason::BY_TIMER;
        case SystemSleepWakeupReason::BY_CAN:       return IWakeupReason::BY_CAN;
        case SystemSleepWakeupReason::BY_USB:       return IWakeupReason::BY_USB;
        case SystemSleepWakeupReason::BY_BLE:       return IWakeupReason::BY_BLE;
        case SystemSleepWakeupReason::BY_NFC:       return IWakeupReason::BY_NFC;
        case SystemSleepWakeupReason::BY_NETWORK:   return IWakeupReason::BY_NETWORK;
        case SystemSleepWakeupReason::UNKNOWN:
        default:
            // Check error code for more info?
            if (result.error() != SYSTEM_ERROR_NONE) {
                 Log.error("Sleep returned error code: %d", result.error());
            }
            return IWakeupReason::UNKNOWN;
    }
}

// --- Implement overrides for other methods if added to ISystem ---