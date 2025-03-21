// FlightControl_Demo/test/unit/Driver-_-Kestrel/kestrel_basic_test.cpp
#include "gtest/gtest.h"
#include "fff.h"

// Define FFF globals
DEFINE_FFF_GLOBALS;

// Define fake functions for Wire library
FAKE_VOID_FUNC(Wire_begin);
FAKE_VOID_FUNC(Wire_setClock, unsigned long);
FAKE_VOID_FUNC(Wire_beginTransmission, uint8_t);
FAKE_VALUE_FUNC(uint8_t, Wire_endTransmission, bool);
FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_3args, uint8_t, uint8_t, bool);
FAKE_VALUE_FUNC(uint8_t, Wire_requestFrom_2args, uint8_t, uint8_t);
FAKE_VALUE_FUNC(int, Wire_available);
FAKE_VALUE_FUNC(int, Wire_read);
FAKE_VOID_FUNC(Wire_write_uint8, uint8_t);
FAKE_VOID_FUNC(Wire_write_buffer, const uint8_t*, size_t);

// Create a mock Wire class that delegates to our fake functions
class MockWire {
public:
    void begin() { Wire_begin(); }
    void setClock(unsigned long clock) { Wire_setClock(clock); }
    void beginTransmission(uint8_t address) { Wire_beginTransmission(address); }
    uint8_t endTransmission(bool sendStop = true) { return Wire_endTransmission(sendStop); }
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop) {
        return Wire_requestFrom_3args(address, quantity, sendStop);
    }
    uint8_t requestFrom(uint8_t address, uint8_t quantity) {
        return Wire_requestFrom_2args(address, quantity);
    }
    int available() { return Wire_available(); }
    int read() { return Wire_read(); }
    void write(uint8_t data) { Wire_write_uint8(data); }
    void write(const uint8_t* data, size_t quantity) { Wire_write_buffer(data, quantity); }
};

// Define the global Wire instance
MockWire Wire;

// Test fixture
class BasicTest : public ::testing::Test {
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
TEST_F(BasicTest, GoogleTestWorks) {
    EXPECT_TRUE(true);
}

// A test to verify that FFF is working with our mocks
TEST_F(BasicTest, FFFWorks) {
    // Call a mocked function
    Wire.begin();
    
    // Verify it was called
    EXPECT_EQ(Wire_begin_fake.call_count, 1);
}