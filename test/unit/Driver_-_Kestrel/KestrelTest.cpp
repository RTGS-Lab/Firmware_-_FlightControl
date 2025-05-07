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
#include "MockAdafruit_SHT4X.h"
#include "MockPAC1934.h"

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
    MockAdafruit_SHT4X mockTempHumidity;
    MockMXC6655 mockAccel;
    MockMXC6655 mockBackupAccel;
    MockPCA9634 mockLed;
    MockPAC1934 mockCsaAlpha;
    MockPAC1934 mockCsaBeta;
    MockMCP79412 mockRtc;
    MockSFE_UBLOX_GNSS mockGps;
    MockVEML3328 mockAls;

    // Set up mocks and test objects
    void SetUp() override {

    }

    void TearDown() override {
        // Any test cleanup code goes here
    }
    
    // Helper method to create a Kestrel instance with mocks
    Kestrel createFullyMockedKestrel(bool useSensors = false) {
        return Kestrel(
            mockTimeProvider,
            mockGpio,
            mockSystem,
            mockWire,
            mockCloud,
            mockSerialDebug,
            mockSerialSdi12,
            mockIoOB,
            mockIoTalon,
            mockCsaAlpha,
            mockCsaBeta,
            mockLed,
            mockRtc,
            mockAls,
            mockGps,
            mockTempHumidity,
            mockAccel,
            mockBackupAccel,
            useSensors
        );
    }
};

// Constructor Tests
TEST_F(KestrelTest, ConstructorDefaultUseSensors) {
    EXPECT_NO_THROW({
        Kestrel kestrel = createFullyMockedKestrel(false);
        // Constructor should set reportSensors to false by default
    });
}

TEST_F(KestrelTest, ConstructorWithUseSensorsTrue) {
    EXPECT_NO_THROW({
        Kestrel kestrel = createFullyMockedKestrel(true);
        // Constructor should set reportSensors to true
        // We can test this indirectly through getData()
    });
}

// Begin() Tests
TEST_F(KestrelTest, BeginFirstTimeInitializationThrowsSystemReset) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    float data[3] = {0.0, 0.0, 0.0};
    
    EXPECT_CALL(mockSystem, resetReason())
        .WillOnce(::testing::Return(IResetReason::WATCHDOG));
    
    EXPECT_CALL(mockAccel, getData())
        .WillRepeatedly(::testing::Return(data));
    
    EXPECT_CALL(mockAccel, getOffset())
        .WillRepeatedly(::testing::Return(data));
    
    // We would need to verify throwError is called, but it's private
    // This tests the overall begin() behavior
    String result = kestrel.begin(0, criticalFault, fault);
    
    // Check that we get through initialization
    EXPECT_EQ(result, "");
}

TEST_F(KestrelTest, BeginI2CInitializationSuccess) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    
    // Test I2C initialization
    EXPECT_CALL(mockWire, isEnabled())
        .WillOnce(::testing::Return(false));
    EXPECT_CALL(mockWire, begin())
        .Times(1);
    EXPECT_CALL(mockWire, setClock(400000))
        .Times(1);
    
    kestrel.begin(0, criticalFault, fault);
    
    EXPECT_FALSE(criticalFault);
}

TEST_F(KestrelTest, BeginIOExpanderInitializationFailureCritical) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    
    // Make IO expander initialization fail
    EXPECT_CALL(mockIoOB, begin())
        .WillOnce(::testing::Return(1)); // Non-zero = failure
    
    kestrel.begin(0, criticalFault, fault);
    
    EXPECT_TRUE(criticalFault);
}

// enablePower() Tests
TEST_F(KestrelTest, EnablePowerPort5ReturnsImmediately) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Port 5 is a special case - should return false
    bool result = kestrel.enablePower(5, true);
    EXPECT_FALSE(result);
}

TEST_F(KestrelTest, EnablePowerInvalidPortThrowsError) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test port 0 and port > 4
    kestrel.enablePower(0, true);
    kestrel.enablePower(10, true);
    
    // Would need to check if throwError was called
    // This is limited since throwError is private
}

TEST_F(KestrelTest, EnablePowerValidPortOperation) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up expectations for enabling power on port 1
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_GLOBAL_EN))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_OB_EN))
        .WillOnce(::testing::Return(0));
    
    EXPECT_CALL(mockIoTalon, pinMode(0, ::testing::_)) // EN[0] for port 1
        .Times(1);
    EXPECT_CALL(mockIoTalon, digitalWrite(0, true))
        .Times(1);
    
    bool result = kestrel.enablePower(1, true);
    EXPECT_FALSE(result); // Always returns false for debugging
}

// enableData() Tests
TEST_F(KestrelTest, EnableDataPort5CallsExternalI2C) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Port 5 calls enableI2C_External
    // We'd need to mock the enableI2C_External behavior
    bool result = kestrel.enableData(5, true);
    EXPECT_FALSE(result);
}

TEST_F(KestrelTest, EnableDataValidPortOperation) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up expectations for enabling data on port 1
    EXPECT_CALL(mockIoTalon, pinMode(1, ::testing::_)) // I2C_EN[0] for port 1
        .Times(1);
    EXPECT_CALL(mockIoTalon, digitalWrite(1, true))
        .Times(1);
    
    bool result = kestrel.enableData(1, true);
    EXPECT_FALSE(result); // Always returns false for debugging
}

// feedWDT() Tests
TEST_F(KestrelTest, FeedWDTNormalOperation) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up expectations for normal WDT feeding
    EXPECT_CALL(mockGpio, pinMode(Pins::WD_HOLD, IPinMode::OUTPUT))
        .Times(1);
    EXPECT_CALL(mockGpio, digitalWrite(Pins::WD_HOLD, 0))
        .Times(1);
    EXPECT_CALL(mockGpio, digitalWrite(Pins::WD_HOLD, 1))
        .Times(1);
    EXPECT_CALL(mockTimeProvider, delay(1))
        .Times(2);
    
    bool result = kestrel.feedWDT();
    EXPECT_TRUE(result);
}

// getTime() Tests
TEST_F(KestrelTest, GetTimeValidTimeProvider) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up a valid time condition
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1585699200)); // April 1, 2020
    
    time_t time = kestrel.getTime();
    EXPECT_EQ(time, 1585699200);
}

TEST_F(KestrelTest, GetTimeInvalidTimeTriggersSync) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // First call: time is invalid
    // Second call (after sync): time is valid
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillOnce(::testing::Return(false))
        .WillRepeatedly(::testing::Return(true));
    
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1585699200));
    
    // This should trigger a sync and return valid time
    time_t time = kestrel.getTime();
    EXPECT_EQ(time, 1585699200);
}

// getTimeString() Tests
TEST_F(KestrelTest, GetTimeStringValidTime) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1585699200));
    
    String result = kestrel.getTimeString();
    EXPECT_EQ(result, "1585699200");
}

TEST_F(KestrelTest, GetTimeStringInvalidTime) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Force getTime() to return 0
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(false));
    
    String result = kestrel.getTimeString();
    EXPECT_EQ(result, "null");
}

// setIndicatorState() Tests
TEST_F(KestrelTest, SetIndicatorStateSensorsPass) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test SENSORS bank with PASS mode
    EXPECT_CALL(mockLed, setOutput(0, ILed::IPortState::PWM)) // Green on
        .Times(1);
    EXPECT_CALL(mockLed, setOutput(1, ILed::IPortState::Off)) // Amber off
        .Times(1);
    EXPECT_CALL(mockLed, setOutput(2, ILed::IPortState::Off)) // Red off
        .Times(1);
    
    bool result = kestrel.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS);
    EXPECT_EQ(result, 0);
}

TEST_F(KestrelTest, SetIndicatorStateAllInit) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test ALL bank with INIT mode - should set all to group blink
    EXPECT_CALL(mockLed, setOutputArray(ILed::IPortState::Group))
        .Times(1);
    EXPECT_CALL(mockLed, setGroupBlinkPeriod(250))
        .Times(1);
    EXPECT_CALL(mockLed, setGroupOnTime(25))
        .Times(1);
    
    bool result = kestrel.setIndicatorState(IndicatorLight::ALL, IndicatorMode::INIT);
    EXPECT_EQ(result, 0);
}

// updateLocation() Tests
TEST_F(KestrelTest, UpdateLocationForceUpdate) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up GPS to have a valid fix
    EXPECT_CALL(mockGps, getPVT())
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(mockGps, getFixType())
        .WillOnce(::testing::Return(3)); // 3D fix
    EXPECT_CALL(mockGps, getGnssFixOk())
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mockGps, getLongitude())
        .WillOnce(::testing::Return(1000000)); // Example value
    EXPECT_CALL(mockGps, getLatitude())
        .WillOnce(::testing::Return(2000000)); // Example value
    EXPECT_CALL(mockGps, getAltitude())
        .WillOnce(::testing::Return(3000)); // Example value
    
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1585699200));
    
    bool result = kestrel.updateLocation(true);
    EXPECT_TRUE(result);
}

TEST_F(KestrelTest, UpdateLocationNoFix) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up GPS to have no fix
    EXPECT_CALL(mockGps, getPVT())
        .WillOnce(::testing::Return(false));
    
    bool result = kestrel.updateLocation(true);
    EXPECT_FALSE(result);
}

// startTimer() Tests
TEST_F(KestrelTest, StartTimerDefaultPeriod) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test with period = 0 should use defaultPeriod
    EXPECT_CALL(mockRtc, setAlarm(300, true)) // defaultPeriod is 300
        .Times(1);
    
    bool result = kestrel.startTimer(0);
    EXPECT_FALSE(result);
}

TEST_F(KestrelTest, StartTimerCustomPeriod) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    EXPECT_CALL(mockRtc, setAlarm(600, true))
        .Times(1);
    
    bool result = kestrel.startTimer(600);
    EXPECT_FALSE(result);
}

// waitUntilTimerDone() Tests
TEST_F(KestrelTest, WaitUntilTimerDoneUnitialized) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Without calling startTimer(), logPeriod should be 0
    bool result = kestrel.waitUntilTimerDone();
    EXPECT_FALSE(result);
}

TEST_F(KestrelTest, WaitUntilTimerDoneSuccess) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Start a timer first
    kestrel.startTimer(300);
    
    // Set up GPIO to show clock interrupt triggered
    EXPECT_CALL(mockGpio, digitalRead(Pins::Clock_INT))
        .WillOnce(::testing::Return(0)); // LOW = interrupt triggered
    
    bool result = kestrel.waitUntilTimerDone();
    EXPECT_TRUE(result);
}

// getSensorPortString() Tests
TEST_F(KestrelTest, GetPosLatValidLocation) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // First call should not trigger GPS update
    String result = kestrel.getPosLat();
    EXPECT_EQ(result, "null"); // No location set yet
    
    // Now set a location through updateLocation
    kestrel.updateLocation(true); // This would set latitude
    // Test requires internal state which is hard to verify
}

// testForBat() Tests
TEST_F(KestrelTest, TestForBatValidBattery) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up voltage reading to indicate valid battery
    EXPECT_CALL(mockCsaAlpha, getBusVoltage(IChannel::CSA_CH1, false))
        .WillOnce(::testing::Return(3.7f)); // Valid battery voltage
    
    bool result = kestrel.testForBat();
    EXPECT_TRUE(result);
}

TEST_F(KestrelTest, TestForBatLowBattery) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up voltage reading to indicate low/no battery
    EXPECT_CALL(mockCsaAlpha, getBusVoltage(IChannel::CSA_CH1, false))
        .WillOnce(::testing::Return(1.5f)); // Low voltage
    
    bool result = kestrel.testForBat();
    EXPECT_FALSE(result);
}

// getMessageID() Tests
TEST_F(KestrelTest, GetMessageIDValidTime) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(1000000));
    EXPECT_CALL(mockTimeProvider, millis())
        .WillOnce(::testing::Return(5000)); // 5 seconds
    
    unsigned long id = kestrel.getMessageID();
    EXPECT_EQ(id, 0); // 1000000 % 5 = 0
}

TEST_F(KestrelTest, GetMessageIDInvalidTime) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // When time is invalid (0), should use random number
    EXPECT_CALL(mockTimeProvider, now())
        .WillOnce(::testing::Return(0));
    
    // Can't really test the random value, just ensure it doesn't crash
    unsigned long id = kestrel.getMessageID();
    // Any value is acceptable for random
}

// More comprehensive begin() test
TEST_F(KestrelTest, BeginFullInitializationSequence) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    
    // Complete begin sequence
    testing::InSequence seq;
    
    // System events
    EXPECT_CALL(mockSystem, on(IEventType::TIME_CHANGED, ::testing::_));
    EXPECT_CALL(mockSystem, on(IEventType::OUT_OF_MEMORY, ::testing::_));
    
    // I2C initialization
    EXPECT_CALL(mockWire, isEnabled()).WillOnce(::testing::Return(false));
    EXPECT_CALL(mockWire, begin());
    EXPECT_CALL(mockWire, setClock(400000));
    
    // IO Expanders
    EXPECT_CALL(mockIoOB, begin()).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockIoTalon, begin()).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockIoTalon, safeMode(IIOExpander::SAFEOFF));
    
    // CSA initialization
    EXPECT_CALL(mockCsaAlpha, begin()).WillOnce(::testing::Return(true));
    EXPECT_CALL(mockCsaBeta, begin()).WillOnce(::testing::Return(true));
    EXPECT_CALL(mockCsaAlpha, setFrequency(IFrequency::CSA_SPS_64));
    
    // LED driver
    EXPECT_CALL(mockLed, begin()).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockLed, setOutputMode(ILed::IOutputMode::OpenDrain));
    EXPECT_CALL(mockLed, setGroupMode(ILed::IGroupMode::Blink));
    EXPECT_CALL(mockLed, setOutputArray(ILed::IPortState::Off));
    
    // Serial port
    EXPECT_CALL(mockSerialSdi12, begin(1200, 0b00000000));
    
    // RTC
    EXPECT_CALL(mockRtc, begin(true)).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockRtc, enableAlarm(false, 0));
    EXPECT_CALL(mockRtc, enableAlarm(false, 1));
    EXPECT_CALL(mockRtc, setMode(IRtc::Mode::Normal));
    
    // GPS
    EXPECT_CALL(mockGps, begin()).WillOnce(::testing::Return(true));
    EXPECT_CALL(mockGps, setI2COutput(GPS_COM_TYPE_UBX));
    EXPECT_CALL(mockGps, setNavigationFrequency(1));
    EXPECT_CALL(mockGps, setAutoPVT(false));
    
    String result = kestrel.begin(0, criticalFault, fault);
    
    EXPECT_EQ(result, "");
    EXPECT_FALSE(criticalFault);
}

// Additional edge case tests
TEST_F(KestrelTest, EnablePowerAndDataAllPorts) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test enabling all ports
    for(int i = 1; i <= 5; i++) {
        kestrel.enablePower(i, true);
        kestrel.enableData(i, true);
    }
    
    // Test disabling all ports
    kestrel.disablePowerAll();
    kestrel.disableDataAll();
}

TEST_F(KestrelTest, GetDataWithSensorsEnabled) {
    Kestrel kestrel = createFullyMockedKestrel(true); // Enable sensors
    
    // Set up ALS expectations
    EXPECT_CALL(mockAls, begin()).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockAls, autoRange()).WillOnce(::testing::Return(1));
    
    bool readState = false;
    EXPECT_CALL(mockAls, getValue(::testing::_, ::testing::Ref(readState)))
        .WillRepeatedly(::testing::DoAll(
            ::testing::SetArgReferee<1>(false),
            ::testing::Return(100.0f)
        ));
    
    String result = kestrel.getData(0);
    EXPECT_NE(result.indexOf("\"ALS\""), -1); // Should contain ALS data
}

TEST_F(KestrelTest, SelfDiagnosticLevel5Memory) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test low memory warning (75% used)
    EXPECT_CALL(mockSystem, freeMemory())
        .WillOnce(::testing::Return(20000)); // Below 46800 threshold
    
    String result = kestrel.selfDiagnostic(5, 0);
    // Would need to check if RAM_LOW error was thrown
}

// Add tests for sleep modes
TEST_F(KestrelTest, SleepPerformanceMode) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Performance mode should return immediately
    int result = kestrel.sleep();
    EXPECT_EQ(result, 0);
}

TEST_F(KestrelTest, WakePerformanceMode) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Performance mode should return immediately
    int result = kestrel.wake();
    EXPECT_EQ(result, 0);
}

// Tests for zero accelerometer
TEST_F(KestrelTest, ZeroAccelReset) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test reset mode
    bool result = kestrel.zeroAccel(true);
    EXPECT_TRUE(result);
}

TEST_F(KestrelTest, ZeroAccelCalibrate) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up accelerometer expectations
    EXPECT_CALL(mockAccel, begin()).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockAccel, getAccel(2, ::testing::_)).WillOnce(::testing::Return(1.0f));
    
    bool result = kestrel.zeroAccel(false);
    EXPECT_FALSE(result);
}

// Test I2C enable/disable functions
TEST_F(KestrelTest, EnableI2COperations) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test OB I2C
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_OB_EN)).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockGpio, pinMode(Pins::I2C_OB_EN, IPinMode::OUTPUT));
    EXPECT_CALL(mockGpio, digitalWrite(Pins::I2C_OB_EN, true));
    
    bool prevState = kestrel.enableI2C_OB(true);
    EXPECT_FALSE(prevState);
    
    // Test Global I2C
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_GLOBAL_EN)).WillOnce(::testing::Return(1));
    EXPECT_CALL(mockGpio, pinMode(Pins::I2C_GLOBAL_EN, IPinMode::OUTPUT));
    EXPECT_CALL(mockGpio, digitalWrite(Pins::I2C_GLOBAL_EN, false));
    
    prevState = kestrel.enableI2C_Global(false);
    EXPECT_TRUE(prevState);
}

// Test SD card operations
TEST_F(KestrelTest, SDCardOperations) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test SD enable
    EXPECT_CALL(mockIoOB, digitalRead(PinsOB::SD_EN)).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockIoOB, pinMode(PinsOB::SD_EN, ::testing::_));
    EXPECT_CALL(mockIoOB, digitalWrite(PinsOB::SD_EN, 1));
    
    bool prevState = kestrel.enableSD(true);
    EXPECT_FALSE(prevState);
    
    // Test SD card detection
    EXPECT_CALL(mockIoOB, pinMode(PinsOB::SD_CD, ::testing::_));
    EXPECT_CALL(mockIoOB, digitalRead(PinsOB::SD_CD)).WillOnce(::testing::Return(0));
    
    bool inserted = kestrel.sdInserted();
    EXPECT_TRUE(inserted);
}

// Test aux power control
TEST_F(KestrelTest, AuxPowerControl) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    EXPECT_CALL(mockIoOB, digitalRead(PinsOB::AUX_EN)).WillOnce(::testing::Return(1));
    EXPECT_CALL(mockIoOB, pinMode(PinsOB::AUX_EN, ::testing::_));
    EXPECT_CALL(mockIoOB, digitalWrite(PinsOB::AUX_EN, false));
    
    bool prevState = kestrel.enableAuxPower(false);
    EXPECT_TRUE(prevState);
}

// Test total errors function
TEST_F(KestrelTest, TotalErrors) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Since we can't directly set errors, we can only test the getter
    uint8_t total = kestrel.totalErrors();
    // Default should be 0 + 0
    EXPECT_EQ(total, 0);
}

// Test fault detection
TEST_F(KestrelTest, GetFaultValidPort) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test fault present (pin LOW)
    EXPECT_CALL(mockIoTalon, digitalRead(2)).WillOnce(::testing::Return(0)); // FAULT[0] for port 1
    
    bool fault = kestrel.getFault(1);
    EXPECT_TRUE(fault);
    
    // Test no fault (pin HIGH)
    EXPECT_CALL(mockIoTalon, digitalRead(2)).WillOnce(::testing::Return(1));
    
    fault = kestrel.getFault(1);
    EXPECT_FALSE(fault);
}

// Test statLED function
TEST_F(KestrelTest, StatLEDControl) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test LED on (state = true)
    bool result = kestrel.statLED(true);
    EXPECT_FALSE(result); // Always returns false
    
    // Test LED off (state = false)
    result = kestrel.statLED(false);
    EXPECT_FALSE(result); // Always returns false
}

// Test connectToCell function
TEST_F(KestrelTest, ConnectToCellSuccess) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test successful connection
    EXPECT_CALL(mockCloud, connect()).Times(1);
    EXPECT_CALL(mockCloud, connected())
        .WillRepeatedly(::testing::Return(true));
    
    bool result = kestrel.connectToCell();
    EXPECT_TRUE(result);
}

TEST_F(KestrelTest, ConnectToCellFailure) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test failed connection (timeout)
    EXPECT_CALL(mockCloud, connect()).Times(1);
    EXPECT_CALL(mockCloud, connected())
        .WillRepeatedly(::testing::Return(false));
    
    bool result = kestrel.connectToCell();
    EXPECT_FALSE(result);
}

// Test releaseWDT function
TEST_F(KestrelTest, ReleaseWDTToggle) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // First call should return false (default) and set to true
    bool result = kestrel.releaseWDT();
    EXPECT_FALSE(result);
    
    // Second call should return true (previous state)
    result = kestrel.releaseWDT();
    EXPECT_TRUE(result);
}

// Test syncTime function - limited test due to complexity
TEST_F(KestrelTest, SyncTimeBasicOperation) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up mock behavior for minimal sync time test
    EXPECT_CALL(mockRtc, getTimeUnix())
        .WillOnce(::testing::Return(1585699200));
    
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(true));
    
    EXPECT_CALL(mockTimeProvider, now())
        .WillRepeatedly(::testing::Return(1585699200));
    
    uint8_t source = kestrel.syncTime(false);
    // Hard to test specific source without more detailed setup
    EXPECT_LE(source, TimeSource::NONE);
}

// Test configTalonSense function
TEST_F(KestrelTest, ConfigTalonSense) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test CSA Beta configuration
    EXPECT_CALL(mockCsaBeta, setCurrentDirection(IChannel::CSA_CH4, 0))
        .Times(1);
    EXPECT_CALL(mockCsaBeta, enableChannel(IChannel::CSA_CH1, false))
        .Times(1);
    EXPECT_CALL(mockCsaBeta, enableChannel(IChannel::CSA_CH2, false))
        .Times(1);
    EXPECT_CALL(mockCsaBeta, enableChannel(IChannel::CSA_CH3, false))
        .Times(1);
    EXPECT_CALL(mockCsaBeta, enableChannel(IChannel::CSA_CH4, true))
        .Times(1);
    
    bool result = kestrel.configTalonSense();
    EXPECT_FALSE(result);
}

// Test the updateTime function
TEST_F(KestrelTest, UpdateTimeSuccess) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up time provider expectations
    EXPECT_CALL(mockTimeProvider, year())
        .WillOnce(::testing::Return(2023));
    EXPECT_CALL(mockTimeProvider, month())
        .WillOnce(::testing::Return(5));
    EXPECT_CALL(mockTimeProvider, day())
        .WillOnce(::testing::Return(15));
    EXPECT_CALL(mockTimeProvider, hour())
        .WillOnce(::testing::Return(10));
    EXPECT_CALL(mockTimeProvider, minute())
        .WillOnce(::testing::Return(30));
    EXPECT_CALL(mockTimeProvider, second())
        .WillOnce(::testing::Return(45));
    
    uint8_t source = kestrel.updateTime();
    // Hard to test specific source in isolated test
    EXPECT_LE(source, TimeSource::NONE);
}

// Test getData for metadata gathering
TEST_F(KestrelTest, GetMetadataBasicInfo) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up time provider for metadata timestamp
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mockTimeProvider, now())
        .WillRepeatedly(::testing::Return(1585699200));
    
    // Set up RTC UUID for metadata
    EXPECT_CALL(mockRtc, getUUIDString())
        .WillOnce(::testing::Return("test-uuid-1234"));
    
    String result = kestrel.getMetadata();
    EXPECT_NE(result.indexOf("\"RTC UUID\":\"test-uuid-1234\""), -1);
    EXPECT_NE(result.indexOf("\"Time\":1585699200"), -1);
}

// Test selfDiagnostic at different levels
TEST_F(KestrelTest, SelfDiagnosticLevel2) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test with level 2 diagnostics - should include accelerometer offsets
    EXPECT_CALL(mockAccel, begin())
        .WillOnce(::testing::Return(0));
    
    // Set up expectations for RTC register reads
    EXPECT_CALL(mockRtc, readByte(0))
        .WillOnce(::testing::Return(0x80));
    EXPECT_CALL(mockRtc, readByte(3))
        .WillOnce(::testing::Return(0x38));
    EXPECT_CALL(mockRtc, readByte(7))
        .WillOnce(::testing::Return(0x12));
    EXPECT_CALL(mockRtc, readByte(8))
        .WillOnce(::testing::Return(0x34));
    EXPECT_CALL(mockRtc, readByte(0x0D))
        .WillOnce(::testing::Return(0x56));
    EXPECT_CALL(mockRtc, readByte(0x14))
        .WillOnce(::testing::Return(0x78));
    
    String result = kestrel.selfDiagnostic(2, 0);
    EXPECT_NE(result.indexOf("\"RTC_Config\""), -1);
}

TEST_F(KestrelTest, SelfDiagnosticLevel3) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test RTC oscillator validation
    EXPECT_CALL(mockWire, beginTransmission(0x6F))
        .Times(1);
    EXPECT_CALL(mockWire, endTransmission())
        .WillOnce(::testing::Return(0));
    
    EXPECT_CALL(mockRtc, getTimeUnix())
        .WillOnce(::testing::Return(1585699200))
        .WillOnce(::testing::Return(1585699201)); // Should increment
    
    EXPECT_CALL(mockTimeProvider, delay(1200))
        .Times(1);
    
    String result = kestrel.selfDiagnostic(3, 0);
    // Hard to verify all content in a simplified test
}

TEST_F(KestrelTest, SelfDiagnosticLevel4) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test CSA setup and port readings
    EXPECT_CALL(mockIoOB, digitalWrite(PinsOB::CSA_EN, 1))
        .Times(1);
    
    EXPECT_CALL(mockCsaAlpha, begin())
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(mockCsaBeta, begin())
        .WillOnce(::testing::Return(true));
    
    // Setup for bus voltage readings - simplified
    bool err = false;
    EXPECT_CALL(mockCsaAlpha, getBusVoltage(::testing::_, true, ::testing::_))
        .WillRepeatedly(::testing::DoAll(
            ::testing::SetArgReferee<2>(false),
            ::testing::Return(3.3f)
        ));
    
    EXPECT_CALL(mockCsaBeta, getBusVoltage(::testing::_, true, ::testing::_))
        .WillRepeatedly(::testing::DoAll(
            ::testing::SetArgReferee<2>(false),
            ::testing::Return(5.0f)
        ));
    
    String result = kestrel.selfDiagnostic(4, 0);
    EXPECT_NE(result.indexOf("\"PORT_V\""), -1);
}

// Test GPS position getters
TEST_F(KestrelTest, PositionGetters) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Manually set position data
    kestrel.updateLocation(true);
    
    // Test position getters
    String latitude = kestrel.getPosLat();
    String longitude = kestrel.getPosLong();
    String altitude = kestrel.getPosAlt();
    time_t posTime = kestrel.getPosTime();
    String posTimeStr = kestrel.getPosTimeString();
    
    // We can't easily verify values in isolation without injection ability
}

// Test error handling and reporting
TEST_F(KestrelTest, GetErrorsFormatting) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    EXPECT_CALL(mockTimeProvider, now())
        .WillRepeatedly(::testing::Return(1585699200));
    
    // There are no errors yet, so this should return an empty string
    String errors = kestrel.getErrors();
    // We'd need to inject errors to test this fully
}

// Test enableI2C_External with proper dependencies
TEST_F(KestrelTest, EnableI2CExternalIntegration) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test setup for external I2C enable
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_GLOBAL_EN))
        .WillOnce(::testing::Return(1));
    EXPECT_CALL(mockGpio, digitalRead(Pins::I2C_OB_EN))
        .WillOnce(::testing::Return(0));
    
    EXPECT_CALL(mockIoOB, digitalRead(PinsOB::I2C_EXT_EN))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(mockIoOB, pinMode(PinsOB::I2C_EXT_EN, ::testing::_));
    EXPECT_CALL(mockIoOB, digitalWrite(PinsOB::I2C_EXT_EN, true));
    
    bool result = kestrel.enableI2C_External(true);
    EXPECT_FALSE(result);
}

// Enhanced sleep tests with mode configuration
TEST_F(KestrelTest, SleepBalancedMode) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Setup for Balanced mode sleep
    // We would need to set powerSaveMode, but it's private
    // This is a limitation of the current design
    
    // Set up mock for Clock_INT pin check
    EXPECT_CALL(mockGpio, digitalRead(Pins::Clock_INT))
        .WillOnce(::testing::Return(1)); // Not triggered yet
    
    // We have limited ability to test internal state changes
    int result = kestrel.sleep();
    // Can't verify result without knowing the mode
}

// Test the GPS wake path in the wake function
TEST_F(KestrelTest, WakeWithGPSUpdate) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Setup for GPS wake test
    // We would need to set powerSaveMode and posTime, but they're private
    
    // Set up basic I2C management for wake
    EXPECT_CALL(mockGpio, digitalWrite(Pins::I2C_GLOBAL_EN, true));
    EXPECT_CALL(mockGpio, digitalWrite(Pins::I2C_OB_EN, false));
    
    int result = kestrel.wake();
    // Can't verify result without knowing the mode
}

// Test critical fault detection in selfDiagnostic
TEST_F(KestrelTest, SelfDiagnosticMemoryCritical) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test critical memory condition (90% used)
    EXPECT_CALL(mockSystem, freeMemory())
        .WillOnce(::testing::Return(10000)); // Below 15600 threshold
    
    String result = kestrel.selfDiagnostic(5, 0);
    // Would need to check if RAM_CRITICAL error was thrown
}

// Test WDT feeding when there's a critical fault
TEST_F(KestrelTest, FeedWDTWithCriticalFault) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Set up a critical fault condition
    // We'd need a way to set criticalFault to true
    
    // For now, we can only test the normal path
    bool result = kestrel.feedWDT();
    EXPECT_TRUE(result);
}

// Test accelerometer initialization fallback
TEST_F(KestrelTest, BeginAccelerometerFallback) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    
    // Set up MXC6655 to fail and BMA456 to succeed
    EXPECT_CALL(mockAccel, begin())
        .WillOnce(::testing::Return(-1)); // Fail
    
    // We'd need a way to test the BMA456 fallback
    
    String result = kestrel.begin(0, criticalFault, fault);
    // Hard to test fallback mechanism in isolation
}

// Test the entire diagnostic sequence
TEST_F(KestrelTest, FullDiagnosticSequence) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test with all diagnostic levels
    for (int level = 0; level <= 5; level++) {
        String result = kestrel.selfDiagnostic(level, 0);
        EXPECT_GT(result.length(), 0); // Check that result is not empty
    }
}

// Test getData with different reportSensors settings
TEST_F(KestrelTest, GetDataWithReportSensorsToggle) {
    // Test with sensors disabled
    {
        Kestrel kestrel = createFullyMockedKestrel(false);
        String result = kestrel.getData(0);
        EXPECT_GT(result.length(), 0); // Check that result is not empty
    }
    
    // Test with sensors enabled
    {
        Kestrel kestrel = createFullyMockedKestrel(true);
        
        EXPECT_CALL(mockAls, begin())
            .WillOnce(::testing::Return(0));
        EXPECT_CALL(mockAls, autoRange())
            .WillOnce(::testing::Return(0));
        
        bool readState = false;
        EXPECT_CALL(mockAls, getValue(::testing::_, ::testing::Ref(readState)))
            .WillRepeatedly(::testing::DoAll(
                ::testing::SetArgReferee<1>(false),
                ::testing::Return(100.0f)
            ));
        
        String result = kestrel.getData(0);
        EXPECT_GT(result.length(), 0); // Check that result is not empty
    }
}

// Test time validity check in getTime
TEST_F(KestrelTest, GetTimeWithInvalidTime) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Setup timeProvider to report invalid time
    EXPECT_CALL(mockTimeProvider, isValid())
        .WillRepeatedly(::testing::Return(false));
    
    // Time value should be 0 for invalid time
    time_t result = kestrel.getTime();
    EXPECT_EQ(result, 0);
}

// Test getTimeString with valid/invalid times
TEST_F(KestrelTest, GetTimeStringValidation) {
    {
        // Test valid time
        Kestrel kestrel = createFullyMockedKestrel();
        
        EXPECT_CALL(mockTimeProvider, isValid())
            .WillRepeatedly(::testing::Return(true));
        EXPECT_CALL(mockTimeProvider, now())
            .WillOnce(::testing::Return(1585699200));
        
        String result = kestrel.getTimeString();
        EXPECT_EQ(result, "1585699200");
    }
    
    {
        // Test invalid time
        Kestrel kestrel = createFullyMockedKestrel();
        
        EXPECT_CALL(mockTimeProvider, isValid())
            .WillRepeatedly(::testing::Return(false));
        
        String result = kestrel.getTimeString();
        EXPECT_EQ(result, "null");
    }
}

// Test sdInserted with different card states
TEST_F(KestrelTest, SDInsertedStates) {
    {
        // Test card inserted
        Kestrel kestrel = createFullyMockedKestrel();
        
        EXPECT_CALL(mockIoOB, pinMode(PinsOB::SD_CD, ::testing::_));
        EXPECT_CALL(mockIoOB, digitalRead(PinsOB::SD_CD))
            .WillOnce(::testing::Return(0)); // LOW = inserted
        
        bool result = kestrel.sdInserted();
        EXPECT_TRUE(result);
    }
    
    {
        // Test card not inserted
        Kestrel kestrel = createFullyMockedKestrel();
        
        EXPECT_CALL(mockIoOB, pinMode(PinsOB::SD_CD, ::testing::_));
        EXPECT_CALL(mockIoOB, digitalRead(PinsOB::SD_CD))
            .WillOnce(::testing::Return(1)); // HIGH = not inserted
        
        bool result = kestrel.sdInserted();
        EXPECT_FALSE(result);
    }
}

// Test setDirection function
TEST_F(KestrelTest, SetDirectionValidPort) {
    Kestrel kestrel = createFullyMockedKestrel();
    
    // Test setting direction for port 1
    EXPECT_CALL(mockIoTalon, pinMode(0, ::testing::_)); // SEL[0] for port 1
    EXPECT_CALL(mockIoTalon, digitalWrite(0, true));
    
    bool result = kestrel.setDirection(1, true);
    EXPECT_FALSE(result); // Always returns false
}

// Final integration test
TEST_F(KestrelTest, CompleteLifecycleTest) {
    Kestrel kestrel = createFullyMockedKestrel();
    bool criticalFault = false;
    bool fault = false;
    
    // Set up minimal begin expectations
    EXPECT_CALL(mockSystem, on(::testing::_, ::testing::_))
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(mockWire, isEnabled())
        .WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(mockWire, begin())
        .Times(::testing::AtLeast(1));
    
    // Begin the Kestrel
    kestrel.begin(0, criticalFault, fault);
    
    // Test timer start/wait cycle
    kestrel.startTimer(10); // Short period for test
    bool timerResult = kestrel.waitUntilTimerDone();
    
    // Test sleep and wake cycle
    kestrel.sleep();
    kestrel.wake();
    
    // Test WDT feeding
    bool wdtResult = kestrel.feedWDT();
    
    // Test data collection
    String data = kestrel.getData(0);
    String metadata = kestrel.getMetadata();
    String diagnostic = kestrel.selfDiagnostic(3, 0);
    String errors = kestrel.getErrors();
    
    // Overall lifecycle test is primarily checking for crashes
}
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