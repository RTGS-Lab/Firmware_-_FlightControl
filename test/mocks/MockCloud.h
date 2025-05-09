#ifndef MOCK_CLOUD_H
#define MOCK_CLOUD_H

#include <gmock/gmock.h>
#include "ICloud.h" // Include the interface definition

/**
 * @brief Google Mock implementation of ICloud for testing.
 */
class MockCloud : public ICloud {
public:
    // Mock all methods defined in the interface
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, disconnect, (const ICloudDisconnectOptions& options), (override));
    MOCK_METHOD(bool, connected, (), (override));
    MOCK_METHOD(bool, syncTime, (), (override));
    MOCK_METHOD(bool, syncTimePending, (), (override));
    MOCK_METHOD(bool, syncTimeDone, (), (override));
    MOCK_METHOD(bool, process, (), (override));
};

#endif // MOCK_CLOUD_H