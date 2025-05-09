#include <gmock/gmock.h>
#include "Sensor.h"

class MockSensor : public Sensor {
    public:
        MOCK_METHOD(String, begin, (time_t time, bool &criticalFault, bool &fault));
        MOCK_METHOD(String, getErrors, (), (override));
        MOCK_METHOD(uint8_t, totalErrors, (), (override));
        MOCK_METHOD(String, getData, (time_t time), (override));
        MOCK_METHOD(String, getMetadata, (), (override));
        MOCK_METHOD(String, selfDiagnostic, (uint8_t diagnosticLevel, time_t time), (override));
        MOCK_METHOD(uint8_t, getTalonPort, ());
        MOCK_METHOD(uint8_t, getSensorPort, ());
        MOCK_METHOD(void, setTalonPort, (uint8_t port));
        MOCK_METHOD(void, setSensorPort, (uint8_t port));
        MOCK_METHOD(bool, isPresent, (), (override));
        MOCK_METHOD(int, sleep, (), (override));
        MOCK_METHOD(int, wake, (ITimeProvider& timeProvider), (override));
    };