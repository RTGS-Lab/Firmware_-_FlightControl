#include "ParticleSystem.h"
// #include <map> // Only needed if managing multiple 'on' handlers complexly

// --- Method Implementations ---

void ParticleSystem::on(IEventType eventType, SystemEventHandler handler) {
    system_event_t particleEvent;
    switch (eventType) {
        case IEventType::TIME_CHANGED:  particleEvent = time_changed; break;
        case IEventType::OUT_OF_MEMORY: particleEvent = out_of_memory; break;
        default: Log.error("Unsupported IEventType in ParticleSystem::on"); return;
    }

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
    switch (particleReason) {
        case RESET_REASON_NONE: return IResetReason::RESET_REASON_NONE;
        case RESET_REASON_UNKNOWN: return IResetReason::RESET_REASON_UNKNOWN;
        // Hardware
        case RESET_REASON_PIN_RESET: return IResetReason::RESET_REASON_PIN_RESET;
        case RESET_REASON_POWER_MANAGEMENT: return IResetReason::RESET_REASON_POWER_MANAGEMENT;
        case RESET_REASON_POWER_DOWN: return IResetReason::RESET_REASON_POWER_DOWN;
        case RESET_REASON_POWER_BROWNOUT: return IResetReason::RESET_REASON_POWER_BROWNOUT;
        case RESET_REASON_WATCHDOG: return IResetReason::RESET_REASON_WATCHDOG;
        // Software
        case RESET_REASON_UPDATE: return IResetReason::RESET_REASON_UPDATE;
        case RESET_REASON_UPDATE_ERROR: return IResetReason::RESET_REASON_UPDATE_ERROR;
        case RESET_REASON_UPDATE_TIMEOUT: return IResetReason::RESET_REASON_UPDATE_TIMEOUT;
        case RESET_REASON_FACTORY_RESET: return IResetReason::RESET_REASON_FACTORY_RESET;
        case RESET_REASON_SAFE_MODE: return IResetReason::RESET_REASON_SAFE_MODE;
        case RESET_REASON_DFU_MODE: return IResetReason::RESET_REASON_DFU_MODE;
        case RESET_REASON_PANIC: return IResetReason::RESET_REASON_PANIC;
        case RESET_REASON_USER: return IResetReason::RESET_REASON_USER;
        case RESET_REASON_CONFIG_UPDATE: return IResetReason::RESET_REASON_CONFIG_UPDATE;
        default: return IResetReason::RESET_REASON_UNKNOWN;
    }
}
uint32_t ParticleSystem::freeMemory() {
    return System.freeMemory();
}

// Implementation for the sleep function with all wakeup reasons
IWakeupReason ParticleSystem::sleep(const particle::SystemSleepConfiguration& config) {
    // Call the actual Particle function
    SystemSleepResult result = System.sleep(config);

    // Translate Particle's SystemSleepWakeupReason to our IWakeupReason
    switch (result.wakeupReason()) {
        case SystemSleepWakeupReason::UNKNOWN:
            return IWakeupReason::UNKNOWN;
        case SystemSleepWakeupReason::BY_GPIO:
            return IWakeupReason::BY_GPIO;
        case SystemSleepWakeupReason::BY_ADC:
            return IWakeupReason::BY_ADC;
        case SystemSleepWakeupReason::BY_DAC:
            return IWakeupReason::BY_DAC;
        case SystemSleepWakeupReason::BY_RTC:
            return IWakeupReason::BY_RTC;
        case SystemSleepWakeupReason::BY_LPCOMP:
            return IWakeupReason::BY_LPCOMP;
        case SystemSleepWakeupReason::BY_USART:
            return IWakeupReason::BY_USART;
        case SystemSleepWakeupReason::BY_I2C:
            return IWakeupReason::BY_I2C;
        case SystemSleepWakeupReason::BY_SPI:
            return IWakeupReason::BY_SPI;
        case SystemSleepWakeupReason::BY_TIMER:
            return IWakeupReason::BY_TIMER;
        case SystemSleepWakeupReason::BY_CAN:
            return IWakeupReason::BY_CAN;
        case SystemSleepWakeupReason::BY_USB:
            return IWakeupReason::BY_USB;
        case SystemSleepWakeupReason::BY_BLE:
            return IWakeupReason::BY_BLE;
        case SystemSleepWakeupReason::BY_NFC:
            return IWakeupReason::BY_NFC;
        case SystemSleepWakeupReason::BY_NETWORK:
            return IWakeupReason::BY_NETWORK;
        default:
            // You might also want to check result.error() here if needed
            return IWakeupReason::UNKNOWN;
    }
}

// --- Implement overrides for other methods if added to ISystem ---