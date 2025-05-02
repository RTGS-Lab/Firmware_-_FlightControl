#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include platform interface mocks
#include "MockTimeProvider.h"
#include "MockGpio.h"
#include "MockSystem.h"
#include "MockWire.h"
#include "MockCloud.h"
#include "MockSerial.h"

// Include hardware component mocks
#include "MockPCAL9535A.h"
#include "MockPAC1934.h"
#include "MockPCA9634.h"
#include "MockMCP79412.h"
#include "MockVEML3328.h"
#include "MockSFE_UBLOX_GNSS.h"
#include "MockMXC6655.h"
#include "MockAdafruit_SHT4X.h"
#include "MockBMA456.h"

//include mock Particle

// Include the Kestrel header
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
    
    // Hardware component mocks
    MockPCAL9535A mockIoOB;
    MockPCAL9535A mockIoTalon;
    MockPAC1934 mockCsaAlpha;
    MockPAC1934 mockCsaBeta;
    MockPCA9634 mockLed;
    MockMCP79412 mockRtc;
    MockVEML3328 mockAls;
    MockSFE_UBLOX_GNSS mockGps;
    MockAdafruit_SHT4X mockHumidityTemp;
    MockMXC6655 mockAccel;
    MockBMA456 mockBackupAccel;
    

    void SetUp() override {
        // Set up default mock behavior
        EXPECT_CALL(mockWire, isEnabled())
            .WillRepeatedly(::testing::Return(false));
    }
};

// Test Kestrel constructor
TEST_F(KestrelTest, TestKestrelConstructor) {
    // Create a Kestrel instance with our mocks
    Kestrel kestrel(mockTimeProvider, 
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
                    mockHumidityTemp,
                    mockAccel,
                    mockBackupAccel,
                    false);
    
    // Verify basic properties
    EXPECT_EQ(kestrel.sensorInterface, BusType::CORE);
    EXPECT_EQ(kestrel.wake(), 0);
}

/*
// Remaining test outline for future implementation

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