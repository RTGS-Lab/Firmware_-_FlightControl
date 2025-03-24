// FlightControl_Demo/test/unit/Driver-_-Kestrel/Driver-_-KestrelTest.cpp
#include "gtest/gtest.h"
#include "fff.h"
#include "MockWireDeclare.h"
#include "MockArduino.h"
#include "MockPCAL9535A.h"
#include "MockMCP79412.h"
#include "MockPCA9634.h"
#include "MockVEML3328.h"
#include "MockPAC1934.h"
#include "MockSHT4x.h"
#include "MockMXC6655.h"
#include "MockBMA456.h"
#include "MockGNSS.h"
#include "MockSensor.h"

// Test fixture for basic mock testing
class KestrelBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset all Wire fakes before each test
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
        
        // Reset Arduino fakes
        RESET_FAKE(pinMode);
        RESET_FAKE(digitalWrite);
        RESET_FAKE(digitalRead);
        RESET_FAKE(millis);
        RESET_FAKE(delay);
        
        // Reset PCAL9535A fakes
        RESET_FAKE(PCAL9535A_begin);
        RESET_FAKE(PCAL9535A_pinMode);
        RESET_FAKE(PCAL9535A_digitalWrite);
        RESET_FAKE(PCAL9535A_digitalRead);
        
        // Reset MCP79412 fakes
        RESET_FAKE(MCP79412_begin);
        RESET_FAKE(MCP79412_getTime);
        RESET_FAKE(MCP79412_setTime);
        
        // Reset PCA9634 fakes
        RESET_FAKE(PCA9634_begin);
        RESET_FAKE(PCA9634_setBrightness);
        RESET_FAKE(PCA9634_setLEDOutputMode);
        
        // Reset VEML3328 fakes
        RESET_FAKE(VEML3328_begin);
        RESET_FAKE(VEML3328_readRed);
        RESET_FAKE(VEML3328_readGreen);
        RESET_FAKE(VEML3328_readBlue);
        
        // Reset PAC1934 fakes
        RESET_FAKE(PAC1934_begin);
        RESET_FAKE(PAC1934_readVoltage);
        RESET_FAKE(PAC1934_readCurrent);
        
        // Reset SHT4x fakes
        RESET_FAKE(SHT4x_begin);
        RESET_FAKE(SHT4x_readTemperature);
        RESET_FAKE(SHT4x_readHumidity);
        
        // Reset MXC6655 fakes
        RESET_FAKE(MXC6655_begin);
        RESET_FAKE(MXC6655_readAcceleration);
        
        // Reset BMA456 fakes
        RESET_FAKE(BMA456_begin);
        RESET_FAKE(BMA456_readAcceleration);
        RESET_FAKE(BMA456_getStepCount);
        
        // Reset GNSS fakes
        RESET_FAKE(GNSS_begin);
        RESET_FAKE(GNSS_getLatitude);
        RESET_FAKE(GNSS_getLongitude);
        RESET_FAKE(GNSS_getAltitude);
        
        // Set default mock behavior
        PCAL9535A_begin_fake.return_val = true;
        MCP79412_begin_fake.return_val = true;
        MCP79412_getTime_fake.return_val = 1616161616; // Fixed timestamp for testing
        Wire_endTransmission_fake.return_val = 0; // Success
        Wire_available_fake.return_val = 1; // Data available
        
        PCA9634_begin_fake.return_val = true;
        VEML3328_begin_fake.return_val = true;
        PAC1934_begin_fake.return_val = true;
        SHT4x_begin_fake.return_val = true;
        MXC6655_begin_fake.return_val = true;
        BMA456_begin_fake.return_val = true;
        GNSS_begin_fake.return_val = true;
        
        // Set up return values for common functions
        VEML3328_readRed_fake.return_val = 500;
        VEML3328_readGreen_fake.return_val = 600;
        VEML3328_readBlue_fake.return_val = 400;
        
        PAC1934_readVoltage_fake.return_val = 3.3f;
        PAC1934_readCurrent_fake.return_val = 0.1f;
        
        GNSS_getLatitude_fake.return_val = 449673925;  // Minneapolis ~44.96°N
        GNSS_getLongitude_fake.return_val = -932838386; // Minneapolis ~-93.28°E
        GNSS_getAltitude_fake.return_val = 25000;      // 250m above MSL
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

// A test to verify PCAL9535A mock
TEST_F(KestrelBasicTest, PCAL9535AMockWorks) {
    // Create a mock object
    PCAL9535A io;
    
    // Use the mock
    bool result = io.begin();
    io.pinMode(0, OUTPUT);
    io.digitalWrite(0, HIGH);
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(PCAL9535A_begin_fake.call_count, 1);
    EXPECT_EQ(PCAL9535A_pinMode_fake.call_count, 1);
    EXPECT_EQ(PCAL9535A_digitalWrite_fake.call_count, 1);
}

// A test to verify MCP79412 mock
TEST_F(KestrelBasicTest, MCP79412MockWorks) {
    // Create a mock object
    MCP79412 rtc;
    
    // Use the mock
    bool beginResult = rtc.begin();
    time_t time = rtc.getTime();
    
    // Verify calls
    EXPECT_TRUE(beginResult);
    EXPECT_EQ(MCP79412_begin_fake.call_count, 1);
    EXPECT_EQ(MCP79412_getTime_fake.call_count, 1);
    EXPECT_EQ(time, 1616161616);
}

// Tests for the PCA9634 mock
TEST_F(KestrelBasicTest, PCA9634MockWorks) {
    // Create a mock object
    PCA9634 led;
    
    // Use the mock
    bool result = led.begin();
    led.setLEDOutputMode(1, 0);
    led.setBrightness(1, 100);
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(PCA9634_begin_fake.call_count, 1);
    EXPECT_EQ(PCA9634_setLEDOutputMode_fake.call_count, 1);
    EXPECT_EQ(PCA9634_setBrightness_fake.call_count, 1);
}

// Tests for the VEML3328 mock
TEST_F(KestrelBasicTest, VEML3328MockWorks) {
    // Create a mock object
    VEML3328 als;
    
    // Use the mock
    bool result = als.begin();
    uint16_t red = als.readRed();
    uint16_t green = als.readGreen();
    uint16_t blue = als.readBlue();
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(VEML3328_begin_fake.call_count, 1);
    EXPECT_EQ(VEML3328_readRed_fake.call_count, 1);
    EXPECT_EQ(VEML3328_readGreen_fake.call_count, 1);
    EXPECT_EQ(VEML3328_readBlue_fake.call_count, 1);
    EXPECT_EQ(red, 500);
    EXPECT_EQ(green, 600);
    EXPECT_EQ(blue, 400);
}

// Tests for the PAC1934 mock
TEST_F(KestrelBasicTest, PAC1934MockWorks) {
    // Create a mock object
    PAC1934 csa;
    
    // Use the mock
    bool result = csa.begin();
    float voltage = csa.readVoltage(0);
    float current = csa.readCurrent(0);
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(PAC1934_begin_fake.call_count, 1);
    EXPECT_EQ(PAC1934_readVoltage_fake.call_count, 1);
    EXPECT_EQ(PAC1934_readCurrent_fake.call_count, 1);
    EXPECT_FLOAT_EQ(voltage, 3.3f);
    EXPECT_FLOAT_EQ(current, 0.1f);
}

// Tests for the SHT4x mock
TEST_F(KestrelBasicTest, SHT4xMockWorks) {
    // Create a mock object
    Adafruit_SHT4x sht;
    
    // Set test values
    sht.temperature = 22.5f;
    sht.humidity = 45.0f;
    
    // Use the mock
    bool result = sht.begin();
    float temp = sht.readTemperature();
    float humid = sht.readHumidity();
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(SHT4x_begin_fake.call_count, 1);
    EXPECT_EQ(SHT4x_readTemperature_fake.call_count, 1);
    EXPECT_EQ(SHT4x_readHumidity_fake.call_count, 1);
    EXPECT_FLOAT_EQ(temp, 22.5f);
    EXPECT_FLOAT_EQ(humid, 45.0f);
}

// Tests for the MXC6655 mock
TEST_F(KestrelBasicTest, MXC6655MockWorks) {
    // Create a mock object
    MXC6655 accel;
    
    // Set test values in mock object
    accel.x_acceleration = 0.1f;
    accel.y_acceleration = -0.2f;
    accel.z_acceleration = 0.98f;
    
    // Use the mock
    bool result = accel.begin();
    float x, y, z;
    bool readResult = accel.readAcceleration(&x, &y, &z);
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(MXC6655_begin_fake.call_count, 1);
    EXPECT_EQ(MXC6655_readAcceleration_fake.call_count, 1);
    EXPECT_FLOAT_EQ(x, 0.1f);
    EXPECT_FLOAT_EQ(y, -0.2f);
    EXPECT_FLOAT_EQ(z, 0.98f);
}

// Tests for the BMA456 mock
TEST_F(KestrelBasicTest, BMA456MockWorks) {
    // Access the global mock object
    BMA456 accel = accel_bma456;
    
    // Set test values
    accel.x_acceleration = 0.05f;
    accel.y_acceleration = 0.1f;
    accel.z_acceleration = 0.95f;
    accel.step_count = 1234;
    BMA456_getStepCount_fake.return_val = 1234; // Set the return value for the fake function
    
    // Use the mock
    bool result = accel.begin();
    float x, y, z;
    bool readResult = accel.readAcceleration(&x, &y, &z);
    uint32_t steps = accel.getStepCount();
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(BMA456_begin_fake.call_count, 1);
    EXPECT_EQ(BMA456_readAcceleration_fake.call_count, 1);
    EXPECT_EQ(BMA456_getStepCount_fake.call_count, 1);
    EXPECT_FLOAT_EQ(x, 0.05f);
    EXPECT_FLOAT_EQ(y, 0.1f);
    EXPECT_FLOAT_EQ(z, 0.95f);
    EXPECT_EQ(steps, 1234);
}

// Tests for the GNSS mock
TEST_F(KestrelBasicTest, GNSSMockWorks) {
    // Create a mock object
    SFE_UBLOX_GNSS gps;
    
    // Use the mock
    bool result = gps.begin();
    long lat = gps.getLatitude();
    long lon = gps.getLongitude();
    long alt = gps.getAltitude();
    
    // Verify calls
    EXPECT_TRUE(result);
    EXPECT_EQ(GNSS_begin_fake.call_count, 1);
    EXPECT_EQ(GNSS_getLatitude_fake.call_count, 1);
    EXPECT_EQ(GNSS_getLongitude_fake.call_count, 1);
    EXPECT_EQ(GNSS_getAltitude_fake.call_count, 1);
    EXPECT_EQ(lat, 449673925);
    EXPECT_EQ(lon, -932838386);
    EXPECT_EQ(alt, 25000);
}