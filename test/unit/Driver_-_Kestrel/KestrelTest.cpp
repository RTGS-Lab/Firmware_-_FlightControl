#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include our mock headers first
#include "Particle.h"
#include "MockSensor.h"
//#include "PCAL9535A.h" // Uncomment if you need to use the actual PCAL9535A
//#include "MockPAC9634.h" // Uncomment if you need to use the actual PAC1934
#include "MockMCP79412.h"
//#include "MockSparkFun_u_blox_GNSS.h" // Uncomment if you need to use the actual SFE_UBLOX_GNSS
//#include "MockPAC1934.h" // Uncomment if you need to use the actual PAC1934
#include "MockVEML3328.h"
//#include "MockAdafruit_SHT4x.h" // Uncomment if you need to use the actual Adafruit_SHT4x
#include "MockMXC6655.h"
//#include "MockBMA456.h" // Uncomment if you need to use the actual BMA456


// This must be included AFTER mocks to ensure the mocks are used
// instead of actual hardware implementations
#include "Kestrel.h"

// Forward declarations for other mocked dependencies needed
class MockSensor;
//class MockPCAL9535A;
//class MockPAC1934; // Uncomment if you need to use the actual PAC1934
class MockMCP79412;
//class MockSFE_UBLOX_GNSS; // Uncomment if you need to use the actual SFE_UBLOX_GNSS
//class MockPAC1934; // Uncomment if you need to use the actual PAC1934
class MockVEML3328;
//class MockAdafruit_SHT4x; // Uncomment if you need to use the actual Adafruit_SHT4x
class MockMXC6655;
//class MockBMA456; // Uncomment if you need to use the actual BMA456


// Test fixture class for Kestrel tests
class KestrelTest : public ::testing::Test {
protected:
    // Set up mocks and test objects
    void SetUp() override {
        // Any test setup code goes here
    }

    void TearDown() override {
        // Any test cleanup code goes here
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
    float value = veml.GetValue(VEML3328::Channel::Red, state);
    EXPECT_FLOAT_EQ(value, 123.45f);
}




// Kestrel begin tests

// Verify Kestrel function calling works
TEST_F(KestrelTest, TestKestrelBegin) {
    // Mock the components that Kestrel depends on
    MockMXC6655 mockAccel;

    EXPECT_CALL(mockAccel, begin())
        .Times(1)
        .WillOnce(::testing::Return(0));  // Return success

    EXPECT_CALL(mockAccel, getAccel(2, 0))
        .Times(1)
        .WillOnce(::testing::Return(20.0f));  // Return success

    Kestrel kestrel(&mockAccel);

    bool result = kestrel.zeroAccel(false); // Reset accelerometer offsets
    
    // Verify the result and state
    EXPECT_FALSE(result);
}

    // Verify Kestrel begin thows error with reset reason if init is not done

    // Verify Kestrel begin sets criticalFault tue if ioOb.begin() fails

    // Verify Kestrel begin sets criticalFault true if ioTalon.begin() fails

    // Verify Kestrel begin sets criticalFault true if csaAlpha.begin() fails TODO: NOT IMPLEMENTED

    // Verify Kestrel begin sets criticalFault true if csaBeta.begin() fails TODO: NOT IMPLEMENTED

    // Verify Kestrel begin sets criticalFault true if led.begin() fails TODO: NOT IMPLEMENTED

    // Verify Kestrel Begin sets led functions if init is not done

    // Verify Kestrel begin sets criticalFault true if rtc.begin() fails

    // Verify Kestel begin sets criticalFault true if gps.begin() fails

    // Verify Kestrel begin sets initDone true if init is done

//Kestrel getErrors tests

//Kestrel getData tests

//Kestrel getMetadata tests

//Kestrel selfDiagnostic tests

//Kestrel updateLocation tests

//Kestrel connectToCell tests

//Kestrel enablePower tests

//Kestrel enableData tests

//Kestrel setDirection tests

//Kestrel getFault tests

//Kestrel enableI2C_OB tests

//Kestrel enableI2C_Global tests

//Kestrel enableI2C_External tests

//Kestrel enableSD tests

//Kestrel sdInserted tests

//Kestrel enableAuxPower tests

//Kestrel getTime tests

//Kestrel syncTime tests

//Kestrel startTimer tests

//Kestrel waitUntilTimerDone tests

//Kestrel getTimeString tests

//kestrel totalErrors tests

//Kestrel statLED tests

//Kestrel setIndicatorState tests

//Kestrel updateTime tests

//Kestrel feedWDT tests

//Kestrel releaseWDT tests

//Kestrel getPosLat tests

//Kestrel getPosLong tests

//Kestrel getPosAlt tests

//Kestrel getPosTime tests

//Kestrel getPosTimeString tests

//Kestrel configTalonSense tests

//Kestrel getMessageID tests

//Kestrel testForBat tests

//Kestrel zeroAccel tests


