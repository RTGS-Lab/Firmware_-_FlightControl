#ifndef MOCK_SYSTEM_H
#define MOCK_SYSTEM_H

#include <gmock/gmock.h>
#include "ISystem.h" // Include the interface definition

/**
 * @brief Google Mock implementation of ISystem for testing.
 */
class MockSystem : public ISystem {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(void, on, (IEventType event, SystemEventHandler handler), (override));
    MOCK_METHOD(IResetReason, resetReason, (), (override));
    MOCK_METHOD(uint32_t, freeMemory, (), (override));
    MOCK_METHOD(bool, waitForCondition, (std::function<bool()> condition, std::chrono::milliseconds timeout), (override));
    MOCK_METHOD(IWakeupReason, sleep, (const ISleepConfig& config), (override));
};

#endif // MOCK_SYSTEM_H