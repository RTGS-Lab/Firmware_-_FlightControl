# Hardware Interfaces and Mocks for Kestrel Testing

## Current State

The Kestrel class uses direct instantiation of hardware components, making them difficult to mock for unit testing. While we've successfully abstracted platform dependencies (TimeProvider, Gpio, System, etc.), hardware components remain tightly coupled.

## Recommended Hardware Interface Abstractions

To enable comprehensive unit testing, we should create interfaces for major hardware components. Here's a blueprint for implementing these interfaces:

### 1. IO Expander Interface

```cpp
// IPCAL9535A.h
class IPCAL9535A {
public:
    virtual ~IPCAL9535A() = default;
    virtual int begin() = 0;
    virtual int pinMode(int Pin, uint8_t State) = 0;
    virtual int digitalWrite(int Pin, bool State) = 0;
    virtual int digitalRead(int Pin) = 0;
    virtual void safeMode(int state) = 0;
    // Add other required methods
};
```

### 2. LED Driver Interface

```cpp
// IPCA9634.h
class IPCA9634 {
public:
    virtual ~IPCA9634() = default;
    virtual int begin() = 0;
    virtual int setOutputMode(PCA9634::OutputMode State) = 0;
    virtual int setGroupMode(PCA9634::GroupMode State) = 0;
    virtual int setOutputArray(PCA9634::PortState Val) = 0;
    virtual int setBrightnessArray(float Brightness) = 0;
    virtual int setGroupBlinkPeriod(uint16_t Period) = 0;
    virtual int setGroupOnTime(uint16_t Period) = 0;
    // Add other required methods
};
```

### 3. RTC Interface

```cpp
// IMCP79412.h
class IMCP79412 {
public:
    virtual ~IMCP79412() = default;
    virtual int begin(bool exOsc) = 0;
    virtual time_t getTimeUnix() = 0;
    virtual String getUUIDString() = 0;
    virtual int setMode(MCP79412::Mode mode) = 0;
    virtual int enableAlarm(bool enable, uint8_t alarmNum) = 0;
    // Add other required methods
};
```

### 4. Current Sensor Interface

```cpp
// IPAC1934.h
class IPAC1934 {
public:
    virtual ~IPAC1934() = default;
    virtual bool begin() = 0;
    virtual void setAddress(uint8_t addr) = 0;
    virtual void setFrequency(PAC1934::Frequency freq) = 0;
    // Add other required methods
};
```

### 5. GPS Interface

```cpp
// ISFE_UBLOX_GNSS.h
class ISFE_UBLOX_GNSS {
public:
    virtual ~ISFE_UBLOX_GNSS() = default;
    virtual bool begin() = 0;
    virtual void setI2COutput(uint8_t comType) = 0;
    virtual bool setNavigationFrequency(uint8_t navFreq) = 0;
    virtual void setAutoPVT(bool autoPVT) = 0;
    virtual uint8_t getNavigationFrequency() = 0;
    virtual uint8_t getMeasurementRate() = 0;
    virtual uint8_t getNavigationRate() = 0;
    virtual int16_t getATTroll() = 0;
    virtual int16_t getATTpitch() = 0;
    virtual int16_t getATTheading() = 0;
    // Add other required methods
};
```

## Implementation Strategy

### Step 1: Create Interface Files

Create interface files for each hardware component in a central location:
- `/lib/FlightControl-hardware-dependencies/src/`

### Step 2: Modify Hardware Components

Make each hardware class implement its corresponding interface:

```cpp
// Existing class modifications
class PCAL9535A : public IPCAL9535A {
    // Implementation remains unchanged
};
```

### Step 3: Refactor Kestrel

Update Kestrel's constructor to use interfaces instead of concrete implementations:

```cpp
class Kestrel: public Sensor {
private:
    IPCAL9535A& ioOB;
    IPCAL9535A& ioTalon;
    IPCA9634& led;
    IPAC1934& csaAlpha;
    IPAC1934& csaBeta;
    IMCP79412& rtc;
    ISFE_UBLOX_GNSS& gps;
    // etc.

public:
    Kestrel(ITimeProvider& timeProvider,
            IGpio& gpio,
            ISystem& system,
            IWire& wire,
            ICloud& cloud,
            ISerial& serialDebug,
            ISerial& serialSdi12,
            IPCAL9535A& ioOB,
            IPCAL9535A& ioTalon,
            IPCA9634& led,
            IPAC1934& csaAlpha,
            IPAC1934& csaBeta,
            IMCP79412& rtc,
            ISFE_UBLOX_GNSS& gps,
            // etc.
            bool useSensors = false);
};
```

### Step 4: Create Factory Function

To keep backward compatibility, provide a factory function:

```cpp
// In Kestrel.cpp
Kestrel* createDefaultKestrel(ITimeProvider& timeProvider,
                            IGpio& gpio,
                            ISystem& system,
                            IWire& wire,
                            ICloud& cloud,
                            ISerial& serialDebug,
                            ISerial& serialSdi12,
                            bool useSensors = false) {
    static PCAL9535A ioOB(0x20);
    static PCAL9535A ioTalon(0x21);
    static PCA9634 led(0x52);
    // etc.
    
    return new Kestrel(timeProvider, gpio, system, wire, cloud, 
                       serialDebug, serialSdi12,
                       ioOB, ioTalon, led, csaAlpha, csaBeta, rtc, gps,
                       useSensors);
}
```

### Step 5: Update Tests

Modify the test file to use the interfaces:

```cpp
TEST_F(KestrelTest, TestKestrelBegin) {
    // Create Kestrel with all mocked dependencies
    Kestrel kestrel(mockTimeProvider, mockGpio, mockSystem, mockWire, mockCloud,
                   mockSerialDebug, mockSerialSdi12,
                   mockIoOB, mockIoTalon, mockLed, mockCsaAlpha, mockCsaBeta, 
                   mockRtc, mockGps, false);
    
    // Now you can test the full begin() method
    bool criticalFault = false;
    bool fault = false;
    kestrel.begin(0, criticalFault, fault);
    
    // Assert expectations
    EXPECT_FALSE(criticalFault);
}
```

## Benefits of This Approach

1. **Full Unit Testing**: Enables testing the entire Kestrel class with mocked dependencies
2. **Isolation**: Tests become deterministic, not dependent on hardware
3. **Test Coverage**: Increases test coverage to include hardware interactions
4. **Maintainability**: Makes code more modular and easier to extend

## Incremental Implementation

This refactoring can be implemented incrementally:

1. Start with the most critical components (PCAL9535A, MCP79412)
2. Add interfaces one at a time to limit disruption
3. Update tests progressively as interfaces are added

## Conclusion

By implementing interfaces for hardware components, we can achieve comprehensive unit testing for Kestrel. The platform dependency interfaces already demonstrate the value of this approach, and extending it to hardware components is a logical next step for maintaining a robust, testable codebase.