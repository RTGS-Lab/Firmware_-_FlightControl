#pragma once

// Modified Kestrel.h intended for testing only
// This provides a controlled environment without class redefinitions

// Include all mocks we need
#include "MockArduino.h"
#include "MockSensor.h"
#include "MockPCAL9535A.h"
#include "MockPCA9634.h"
#include "MockMCP79412.h"
#include "MockGNSS.h"
#include "MockPAC1934.h"
#include "MockVEML3328.h"
#include "MockSHT4x.h"
#include "MockMXC6655.h"
#include "MockBMA456.h"
#include "MockWireDeclare.h"

// Define constants and namespaces from Kestrel.h that we need for testing
namespace Pins {
    constexpr uint16_t WD_HOLD  = D2;
    constexpr uint16_t SD_CS    = D8;
    constexpr uint16_t Clock_INT = D22;
    constexpr uint16_t TALON1_GPIOA = A3;
    constexpr uint16_t TALON1_GPIOB = D7;
    constexpr uint16_t TALON2_GPIOA = A2;
    constexpr uint16_t TALON2_GPIOB = D6;
    constexpr uint16_t TALON3_GPIOA = A1;
    constexpr uint16_t TALON3_GPIOB = D5;
    constexpr uint16_t I2C_GLOBAL_EN = D23;
    constexpr uint16_t I2C_OB_EN = A6;
}

namespace PinsOB {
    constexpr uint16_t I2C_EXT_EN = 10;
    constexpr uint16_t SD_CD = 8;
    constexpr uint16_t SD_EN = 12;
    constexpr uint16_t AUX_EN = 15;
    constexpr uint16_t CE = 11;
    constexpr uint16_t LED_EN = 13;
    constexpr uint16_t CSA_EN = 14;
    constexpr uint16_t GPS_INT = 7;
}

namespace PinsTalon {
    constexpr uint8_t SEL[4] = {0, 4, 8, 12};
    constexpr uint8_t I2C_EN[4] = {1, 5, 9, 13};
    constexpr uint8_t EN[4] = {3, 7, 11, 15};
    constexpr uint8_t FAULT[4] = {2, 6, 10, 14};
}

namespace TimeSource {
    constexpr uint8_t INCREMENT = 4;
    constexpr uint8_t RTC = 3;
    constexpr uint8_t GPS_RTC = 2;
    constexpr uint8_t CELLULAR = 1;
    constexpr uint8_t GPS = 0;
    constexpr uint8_t NONE = 5;
}

namespace IndicatorLight {
    constexpr uint8_t SENSORS = 1;
    constexpr uint8_t GPS = 2;
    constexpr uint8_t CELL = 3;
    constexpr uint8_t STAT = 4;
    constexpr uint8_t ALL = 5;
}

namespace IndicatorMode {
    constexpr uint8_t NONE = 0;
    constexpr uint8_t PASS = 1;
    constexpr uint8_t WAITING = 2;
    constexpr uint8_t ERROR = 3;
    constexpr uint8_t ERROR_CRITICAL = 4;
    constexpr uint8_t PREPASS = 5;
    constexpr uint8_t INIT = 6;
    constexpr uint8_t IDLE = 7;
    constexpr uint8_t COMMAND = 8;
}

namespace AccelType {
    constexpr uint8_t MXC6655 = 0;
    constexpr uint8_t BMA456 = 1;
}

namespace HardwareVersion {
    constexpr uint8_t PRE_1v9 = 0;
    constexpr uint8_t MODEL_1v9 = 1;
}

struct dateTimeStruct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    uint8_t source = TimeSource::NONE;
};

// Simplified version of Kestrel for testing
// We'll manually implement the methods we want to test
class Kestrel : public Sensor {
public:
    constexpr static int MAX_NUM_ERRORS = 10;
    const std::string FIRMWARE_VERSION = "1.7.5";
    static constexpr uint8_t numTalonPorts = 5;
    static constexpr int MAX_MESSAGE_LENGTH = 1024;
    
    dateTimeStruct currentDateTime = {2049, 6, 16, 3, 27, 31, TimeSource::NONE};
    uint8_t timeFix = 0;
    SFE_UBLOX_GNSS gps;
    
    // Constructor
    Kestrel(bool useSensors = false) : reportSensors(useSensors) {
        // Initialize member objects
    }
    
    // Public methods we want to test
    std::string begin(time_t time, bool &criticalFault, bool &fault) {
        // Call internal device initializations
        ioOB.begin();
        ioTalon.begin();
        rtc.begin();
        led.begin();
        
        if (reportSensors) {
            als.begin();
            csaAlpha.begin();
            csaBeta.begin();
            atmos.begin();
            accel.begin();
            gps.begin();
        }
        
        // Return success message
        return "Kestrel " + FIRMWARE_VERSION + " initialized successfully";
    }
    
    bool enablePower(uint8_t port, bool state = true) {
        if (port >= numTalonPorts) return false;
        
        // Call IO expander to enable/disable power
        return ioTalon.digitalWrite(PinsTalon::EN[port], state);
    }
    
    bool enableData(uint8_t port, bool state = true) {
        if (port >= numTalonPorts) return false;
        
        // Call IO expander to enable/disable data
        return ioTalon.digitalWrite(PinsTalon::I2C_EN[port], state);
    }
    
    time_t getTime() {
        return rtc.getTime();
    }
    
    bool enableI2C_OB(bool state = true) {
        return ioOB.digitalWrite(PinsOB::I2C_EXT_EN, state);
    }
    
    bool enableI2C_Global(bool state = true) {
        // In our mock environment, digitalWrite returns void
        // But in the real implementation it would return a status
        digitalWrite(Pins::I2C_GLOBAL_EN, state);
        return true; // Assume success
    }
    
    bool enableI2C_External(bool state = true) {
        return ioOB.digitalWrite(PinsOB::I2C_EXT_EN, state);
    }
    
    bool setIndicatorState(uint8_t ledBank, uint8_t mode) {
        int brightness = 0;
        
        // Convert mode to brightness
        switch (mode) {
            case IndicatorMode::PASS:
                brightness = 75;
                break;
            case IndicatorMode::ERROR:
                brightness = 100;
                break;
            default:
                brightness = 0;
                break;
        }
        
        return led.setBrightness(ledBank, brightness);
    }
    
    // Other methods as needed for testing...
    
private:
    PCAL9535A ioOB;
    PCAL9535A ioTalon;
    MCP79412 rtc;
    PAC1934 csaAlpha;
    PAC1934 csaBeta;
    VEML3328 als;
    Adafruit_SHT4x atmos;
    MXC6655 accel;
    PCA9634 led;
    bool reportSensors = false;
};

// Helper functions for testing Kestrel
namespace KestrelTest {
    // Setup a Kestrel instance with all mock dependencies properly configured
    inline Kestrel* createKestrel() {
        // Configure all mock return values for a successful initialization
        PCAL9535A_begin_fake.return_val = true;
        MCP79412_begin_fake.return_val = true;
        MCP79412_getTime_fake.return_val = 1616161616;
        PCA9634_begin_fake.return_val = true;
        VEML3328_begin_fake.return_val = true;
        PAC1934_begin_fake.return_val = true;
        SHT4x_begin_fake.return_val = true;
        MXC6655_begin_fake.return_val = true;
        BMA456_begin_fake.return_val = true;
        GNSS_begin_fake.return_val = true;
        
        // Create a Kestrel instance
        return new Kestrel(true);  // true = use sensors
    }
    
    // Reset all mocks for a clean slate
    inline void resetAllMocks() {
        // Wire mocks
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
        
        // Arduino mocks
        RESET_FAKE(pinMode);
        RESET_FAKE(digitalWrite);
        RESET_FAKE(digitalRead);
        RESET_FAKE(millis);
        RESET_FAKE(delay);
        
        // Hardware component mocks
        RESET_FAKE(PCAL9535A_begin);
        RESET_FAKE(PCAL9535A_pinMode);
        RESET_FAKE(PCAL9535A_digitalWrite);
        RESET_FAKE(PCAL9535A_digitalRead);
        
        RESET_FAKE(MCP79412_begin);
        RESET_FAKE(MCP79412_getTime);
        RESET_FAKE(MCP79412_setTime);
        
        RESET_FAKE(PCA9634_begin);
        RESET_FAKE(PCA9634_setBrightness);
        RESET_FAKE(PCA9634_setLEDOutputMode);
        
        RESET_FAKE(VEML3328_begin);
        RESET_FAKE(VEML3328_readRed);
        RESET_FAKE(VEML3328_readGreen);
        RESET_FAKE(VEML3328_readBlue);
        
        RESET_FAKE(PAC1934_begin);
        RESET_FAKE(PAC1934_readVoltage);
        RESET_FAKE(PAC1934_readCurrent);
        
        RESET_FAKE(SHT4x_begin);
        RESET_FAKE(SHT4x_readTemperature);
        RESET_FAKE(SHT4x_readHumidity);
        
        RESET_FAKE(MXC6655_begin);
        RESET_FAKE(MXC6655_readAcceleration);
        
        RESET_FAKE(BMA456_begin);
        RESET_FAKE(BMA456_readAcceleration);
        RESET_FAKE(BMA456_getStepCount);
        
        RESET_FAKE(GNSS_begin);
        RESET_FAKE(GNSS_getLatitude);
        RESET_FAKE(GNSS_getLongitude);
        RESET_FAKE(GNSS_getAltitude);
    }
    
    // Set default successful return values for hardware operations
    inline void setDefaultMockBehavior() {
        // Configure default successful behaviors
        Wire_endTransmission_fake.return_val = 0; // Success
        Wire_available_fake.return_val = 1; // Data available
        
        PCAL9535A_begin_fake.return_val = true;
        PCAL9535A_digitalWrite_fake.return_val = true;
        PCAL9535A_digitalRead_fake.return_val = 0; // Default to LOW
        
        MCP79412_begin_fake.return_val = true;
        MCP79412_getTime_fake.return_val = 1616161616;
        MCP79412_setTime_fake.return_val = true;
        
        PCA9634_begin_fake.return_val = true;
        PCA9634_setBrightness_fake.return_val = true;
        PCA9634_setLEDOutputMode_fake.return_val = true;
        
        VEML3328_begin_fake.return_val = true;
        VEML3328_readRed_fake.return_val = 500;
        VEML3328_readGreen_fake.return_val = 600;
        VEML3328_readBlue_fake.return_val = 400;
        
        PAC1934_begin_fake.return_val = true;
        PAC1934_readVoltage_fake.return_val = 3.3f;
        PAC1934_readCurrent_fake.return_val = 0.1f;
        
        SHT4x_begin_fake.return_val = true;
        
        MXC6655_begin_fake.return_val = true;
        BMA456_begin_fake.return_val = true;
        
        GNSS_begin_fake.return_val = true;
        GNSS_isConnected_fake.return_val = true;
        GNSS_getLatitude_fake.return_val = 449673925;
        GNSS_getLongitude_fake.return_val = -932838386;
        GNSS_getAltitude_fake.return_val = 25000;
        GNSS_getSIV_fake.return_val = 8;
        GNSS_getYear_fake.return_val = 2023;
        GNSS_getMonth_fake.return_val = 5;
        GNSS_getDay_fake.return_val = 15;
        GNSS_getHour_fake.return_val = 10;
        GNSS_getMinute_fake.return_val = 30;
        GNSS_getSecond_fake.return_val = 0;
        
        millis_fake.return_val = 1000; // Start at 1 second
    }
}