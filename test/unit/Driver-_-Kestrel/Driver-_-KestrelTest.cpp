// FlightControl_Demo/test/unit/Driver-_-Kestrel/kestrel_basic_test.cpp
#include "gtest/gtest.h"
#include "fff.h"
#include "MockWireDeclare.h"

// Test fixture
class KestrelBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset all fakes before each test
        RESET_FAKE(Wire_begin);
        RESET_FAKE(Wire_setClock);
        RESET_FAKE(Wire_beginTransmission);
        RESET_FAKE(Wire_endTransmission);
        RESET_FAKE(Wire_requestFrom_3args);
        RESET_FAKE(Wire_requestFrom_2args);
        RESET_FAKE(Wire_available);
        RESET_FAKE(Wire_read);
        RESET_FAKE(Wire_write_uint8);
        RESET_FAKE(Wire_write_buffer);
    }
};

// A simple test to verify that Google Test is working
TEST_F(KestrelBasicTest, GoogleTestWorks) {
    EXPECT_TRUE(true);
}

// A test to verify that FFF is working with our mocks
TEST_F(KestrelBasicTest, FFFWorks) {
    // Call a mocked function
    Wire.begin();
    
    // Verify it was called
    EXPECT_EQ(Wire_begin_fake.call_count, 1);
}