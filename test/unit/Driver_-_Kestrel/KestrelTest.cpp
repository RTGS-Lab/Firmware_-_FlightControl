#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include our mock headers first
#include "Particle.h"
#include "MockSensor.h"
#include "MockPCAL9535A.h"
#include "MockPCA9634.h"
#include "MockMCP79412.h"
#include "MockSFE_UBLOX_GNSS.h"
#include "MockVEML3328.h"
#include "MockMXC6655.h"

// Include platform interface mocks
#include "MockTimeProvider.h"
#include "MockGpio.h"
#include "MockSystem.h"
#include "MockWire.h"
#include "MockCloud.h"
#include "MockSerial.h"

// This must be included AFTER mocks to ensure the mocks are used
// instead of actual hardware implementations
#include "Kestrel.h"

// Test fixture class for Kestrel tests
class KestrelTest : public ::testing::Test {
protected:
    // Our platform interface mocks
    MockTimeProvider mockTimeProvider;
    MockGpio mockGpio;
    MockSystem mockSystem;
    MockWire mockWire;
    MockCloud mockCloud;
    MockSerial mockSerialDebug;
    MockSerial mockSerialSdi12;
    
    // Additional hardware component mocks that would need injection
    MockPCAL9535A mockIoOB;
    MockPCAL9535A mockIoTalon;
    MockPCA9634 mockLed;
    MockMCP79412 mockRtc;
    MockSFE_UBLOX_GNSS mockGps;
    MockVEML3328 mockAls;
    MockMXC6655 mockAccel;

    // Set up mocks and test objects
    void SetUp() override {
        // Set up default mock behavior here
        EXPECT_CALL(mockWire, isEnabled())
            .WillRepeatedly(::testing::Return(false));
            
        // Add more default behaviors as needed
    }

    void TearDown() override {
        // Any test cleanup code goes here
    }
    
    // Helper method to create a Kestrel instance with mocks
    // Note: This only passes the platform interface mocks, not hardware component mocks
    Kestrel createKestrel() {
        return Kestrel(
            mockTimeProvider,
            mockGpio,
            mockSystem,
            mockWire,
            mockCloud,
            mockSerialDebug,
            mockSerialSdi12,
            false // useSensors
        );
    }
};

// Basic test to verify our testing framework works
TEST_F(KestrelTest, TestSensorMockWorks) {
    MockSensor mockSensor;
    time_t dummyTime = 0;
    bool criticalFault = false;
    bool fault = false;

    EXPECT_CALL(mockSensor, begin(dummyTime, ::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return("Sensor Initialized"));

    String result = mockSensor.begin(dummyTime, criticalFault, fault);
    EXPECT_EQ(result, "Sensor Initialized");
}

// Test that the MCP79412 RTC mock works
TEST_F(KestrelTest, TestMockRTC) {
    MockMCP79412 rtc;
    
    // Set up expectations
    EXPECT_CALL(rtc, begin(true))
        .Times(1)
        .WillOnce(::testing::Return(1));  // Return success
    
    EXPECT_CALL(rtc, getTimeUnix())
        .Times(1)
        .WillOnce(::testing::Return(1585699200));  // Return a fixed time (April 1, 2020)
    
    EXPECT_CALL(rtc, getUUIDString())
        .Times(1)
        .WillOnce(::testing::Return("test-uuid-1234"));
    
    // Test that the mock behaves as expected
    int result = rtc.begin(true);
    EXPECT_EQ(result, 1);
    
    time_t time = rtc.getTimeUnix();
    EXPECT_EQ(time, 1585699200);
    
    String uuid = rtc.getUUIDString();
    EXPECT_EQ(uuid, "test-uuid-1234");
}

// Test that the VEML3328 mock works
TEST_F(KestrelTest, TestMockVEML3328) {
    MockVEML3328 veml;
    
    // Set up expectations
    EXPECT_CALL(veml, begin())
        .Times(1)
        .WillOnce(::testing::Return(0));  // Return success
    
    EXPECT_CALL(veml, autoRange())
        .Times(1)
        .WillOnce(::testing::Return(1));  // Return success
    
    EXPECT_CALL(veml, GetValue(::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(123.45f));  // Return a fixed value
    
    // Test that the mock behaves as expected
    int result = veml.begin();
    EXPECT_EQ(result, 0);
    
    result = veml.autoRange();
    EXPECT_EQ(result, 1);
    
    bool state = false;
    float value = veml.getValue(VEML3328::Channel::Red, state);
    EXPECT_FLOAT_EQ(value, 123.45f);
}

// Test platform interfaces in Kestrel
TEST_F(KestrelTest, TestKestrelPlatformInteractions) {
    // Set up expectations for the Kestrel constructor and begin method
    EXPECT_CALL(mockWire, begin()).Times(1);
    EXPECT_CALL(mockWire, setClock(400000)).Times(1);
    EXPECT_CALL(mockSystem, on(IEventType::TIME_CHANGED, ::testing::_)).Times(1);
    EXPECT_CALL(mockSystem, on(IEventType::OUT_OF_MEMORY, ::testing::_)).Times(1);
    EXPECT_CALL(mockSystem, resetReason()).WillOnce(::testing::Return(IResetReason::POWER_DOWN));
    
    // Additional expectations for hardware components would go here
    // but we can't easily mock them without refactoring Kestrel
    
    // Create a Kestrel instance with our mocked platform interfaces
    Kestrel kestrel = createKestrel();
    
    // This would initialize Kestrel, but we can't fully test it without hardware component mocks
    // bool criticalFault = false;
    // bool fault = false;
    // String result = kestrel.begin(0, criticalFault, fault);
    
    // Instead, we can test isolated methods that primarily use platform interfaces
    // For example, methods that use timeProvider, gpio, etc.
}

// Test platform ITimeProvider interactions
TEST_F(KestrelTest, TestKestrelTimeOperations) {
    // Set up expectations
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1585699200)); // April 1, 2020
        
    // Create Kestrel instance (note: this doesn't test begin())
    Kestrel kestrel = createKestrel();
    
    // Test getTime() which should use mockTimeProvider
    time_t time = kestrel.getTime();
    EXPECT_EQ(time, 1585699200);
}

// Test GPIO interactions
TEST_F(KestrelTest, TestKestrelGpioOperations) {
    // Set up expectations
    EXPECT_CALL(mockGpio, pinMode(Pins::WD_HOLD, IPinMode::OUTPUT)).Times(1);
    EXPECT_CALL(mockGpio, digitalWrite(Pins::WD_HOLD, 1)).Times(1);
    
    // Create Kestrel instance
    Kestrel kestrel = createKestrel();
    
    // Test feedWDT which should use mockGpio
    bool result = kestrel.feedWDT();
    EXPECT_TRUE(result);
}

// More tests would be added to cover other Kestrel methods
// that interact with the platform interfaces we've mocked

/*
// Remaining test outline (commented out since we can't fully implement these yet)

// Kestrel begin tests
// Verify Kestrel begin throws error with reset reason if init is not done
// Verify Kestrel begin sets criticalFault true if ioOb.begin() fails
// Verify Kestrel begin sets criticalFault true if ioTalon.begin() fails
// Verify Kestrel begin sets criticalFault true if csaAlpha.begin() fails
// Verify Kestrel begin sets criticalFault true if csaBeta.begin() fails
// Verify Kestrel begin sets criticalFault true if led.begin() fails
// Verify Kestrel Begin sets led functions if init is not done
// Verify Kestrel begin sets criticalFault true if rtc.begin() fails
// Verify Kestel begin sets criticalFault true if gps.begin() fails
// Verify Kestrel begin sets initDone true if init is done

// Kestrel getErrors tests
// Kestrel getData tests
// Kestrel getMetadata tests
// Kestrel selfDiagnostic tests
// Kestrel updateLocation tests
// Kestrel connectToCell tests
// Kestrel enablePower tests
// Kestrel enableData tests
// Kestrel setDirection tests
// Kestrel getFault tests
// Kestrel enableI2C_OB tests
// Kestrel enableI2C_Global tests
// Kestrel enableI2C_External tests
// Kestrel enableSD tests
// Kestrel sdInserted tests
// Kestrel enableAuxPower tests
// Kestrel getTime tests
// Kestrel syncTime tests
// Kestrel startTimer tests
// Kestrel waitUntilTimerDone tests
// Kestrel getTimeString tests
// Kestrel totalErrors tests
// Kestrel statLED tests
// Kestrel setIndicatorState tests
// Kestrel updateTime tests
// Kestrel feedWDT tests
// Kestrel releaseWDT tests
// Kestrel getPosLat tests
// Kestrel getPosLong tests
// Kestrel getPosAlt tests
// Kestrel getPosTime tests
// Kestrel getPosTimeString tests
// Kestrel configTalonSense tests
// Kestrel getMessageID tests
// Kestrel testForBat tests
// Kestrel zeroAccel tests
*/