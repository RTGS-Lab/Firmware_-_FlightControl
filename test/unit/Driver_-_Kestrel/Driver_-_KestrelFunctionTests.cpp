// FlightControl_Demo/test/unit/Driver-_-Kestrel/KestrelFunctionTests.cpp
// Tests for Kestrel's public functions

#include "gtest/gtest.h"
#include "fff.h"
#include "TestKestrel.h"

// Test fixture for Kestrel public function tests
class KestrelPublicTest : public ::testing::Test {
protected:
    Kestrel* kestrel;

    void SetUp() override {
        // Reset all mocks before each test
        KestrelTest::resetAllMocks();
        
        // Set default mock behaviors
        KestrelTest::setDefaultMockBehavior();
        
        // Create a Kestrel instance with properly configured mocks
        kestrel = KestrelTest::createKestrel();
    }
    
    void TearDown() override {
        // Clean up
        delete kestrel;
    }
};

// Test Kestrel initialization
TEST_F(KestrelPublicTest, BeginTest) {
    // Set up additional mock behaviors if needed
    bool criticalFault = false;
    bool fault = false;
    
    // Call Kestrel's begin method
    std::string result = kestrel->begin(1616161616, criticalFault, fault);
    
    // Verify begin was called on all dependent hardware components
    EXPECT_EQ(PCAL9535A_begin_fake.call_count, 2); // 2 I/O expanders
    EXPECT_EQ(MCP79412_begin_fake.call_count, 1);  // RTC
    EXPECT_EQ(PCA9634_begin_fake.call_count, 1);   // LED controller
    
    // Verify no critical faults occurred
    EXPECT_FALSE(criticalFault);
    EXPECT_FALSE(fault);
    
    // Verify result contains expected success message
    EXPECT_TRUE(result.find("Kestrel") != std::string::npos);
}

// Test port power control
TEST_F(KestrelPublicTest, EnablePowerTest) {
    // Set up mock behavior for I/O expanders
    PCAL9535A_digitalWrite_fake.return_val = true;
    
    // Test enabling power on port 1
    bool result = kestrel->enablePower(1, true);
    
    // Verify the result and calls
    EXPECT_TRUE(result);
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 1);
    
    // Test disabling power
    RESET_FAKE(PCAL9535A_digitalWrite);
    PCAL9535A_digitalWrite_fake.return_val = true;
    
    result = kestrel->enablePower(1, false);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 1);
    
    // Test with invalid port (should return false)
    result = kestrel->enablePower(10, true); // Port 10 is out of range
    EXPECT_FALSE(result);
}

// Test data port control
TEST_F(KestrelPublicTest, EnableDataTest) {
    // Set up mock behavior for I/O expanders
    PCAL9535A_digitalWrite_fake.return_val = true;
    
    // Test enabling data on port 1
    bool result = kestrel->enableData(1, true);
    
    // Verify the result and calls
    EXPECT_TRUE(result);
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 1);
    
    // Test with invalid port (should return false)
    result = kestrel->enableData(10, true); // Port 10 is out of range
    EXPECT_FALSE(result);
}

// Test time retrieval
TEST_F(KestrelPublicTest, GetTimeTest) {
    // Set up mock behavior for time retrieval
    MCP79412_getTime_fake.return_val = 1616161616;
    
    // Call the method under test
    time_t time = kestrel->getTime();
    
    // Verify the result matches what we expect
    EXPECT_EQ(time, 1616161616);
    EXPECT_EQ(MCP79412_getTime_fake.call_count, 1);
}

// Test I2C bus control
TEST_F(KestrelPublicTest, I2CControlTest) {
    // Set up mock behavior
    PCAL9535A_digitalWrite_fake.return_val = true;
    
    // Test enabling I2C buses
    EXPECT_TRUE(kestrel->enableI2C_Global(true));
    EXPECT_TRUE(kestrel->enableI2C_OB(true));
    EXPECT_TRUE(kestrel->enableI2C_External(true));
    
    // Verify the calls occurred
    // Note: Our simplified test implementation uses digitalWrite for Global, not PCAL9535A
    EXPECT_EQ(digitalWrite_fake.call_count, 1); 
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 2);
    
    // Reset the fakes for the next series of tests
    RESET_FAKE(PCAL9535A_digitalWrite);
    RESET_FAKE(digitalWrite);
    PCAL9535A_digitalWrite_fake.return_val = true;
    
    // Test disabling I2C buses
    EXPECT_TRUE(kestrel->enableI2C_Global(false));
    EXPECT_TRUE(kestrel->enableI2C_OB(false));
    EXPECT_TRUE(kestrel->enableI2C_External(false));
    
    // Verify the calls occurred
    EXPECT_EQ(digitalWrite_fake.call_count, 1);
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 2);
}

// Test indicator light control
TEST_F(KestrelPublicTest, IndicatorLightTest) {
    // Set up mock behavior
    PCA9634_setBrightness_fake.return_val = true;
    
    // Test setting lights to different states
    EXPECT_TRUE(kestrel->setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS));
    EXPECT_TRUE(kestrel->setIndicatorState(IndicatorLight::GPS, IndicatorMode::ERROR));
    
    // Verify the right calls were made
    EXPECT_EQ(PCA9634_setBrightness_fake.call_count, 2);
}

// Add more tests for other public Kestrel functions as needed