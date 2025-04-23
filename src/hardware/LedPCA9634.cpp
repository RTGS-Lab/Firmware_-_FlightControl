// src/hardware/LedPCA9634.cpp
#include "LedPCA9634.h"

// Constructor
LedPCA9634::LedPCA9634(int address)
    : driver(address) // Initialize the driver with address
{
    // Constructor body is empty as the initialization is done in the initializer list
}

// Core methods
int LedPCA9634::begin() {
    return driver.begin();
}

int LedPCA9634::sleep(bool State) {
    return driver.sleep(State);
}

// Configuration methods
int LedPCA9634::setOutputMode(IOutputMode State) {
    // Convert from interface enum to driver enum
    OutputMode driverState;
    switch (State) {
        case IOutputMode::OpenDrain:
            driverState = OpenDrain;
            break;
        case IOutputMode::TotemPole:
            driverState = TotemPole;
            break;
        default:
            driverState = OpenDrain; // Default case
            break;
    }
    return driver.setOutputMode(driverState);
}

int LedPCA9634::setGroupMode(IGroupMode State) {
    // Convert from interface enum to driver enum
    GroupMode driverState;
    switch (State) {
        case IGroupMode::Dim:
            driverState = Dim;
            break;
        case IGroupMode::Blink:
            driverState = Blink;
            break;
        default:
            driverState = Dim; // Default case
            break;
    }
    return driver.setGroupMode(driverState);
}

// Group control
int LedPCA9634::setGroupBlinkPeriod(uint16_t Period) {
    return driver.setGroupBlinkPeriod(Period);
}

int LedPCA9634::setGroupOnTime(uint16_t Period) {
    return driver.setGroupOnTime(Period);
}

// Brightness control
int LedPCA9634::setBrightness(uint8_t Pos, float Brightness) {
    return driver.setBrightness(Pos, Brightness);
}

int LedPCA9634::setBrightnessArray(float Brightness) {
    return driver.setBrightnessArray(Brightness);
}

// Output state control
int LedPCA9634::setOutput(uint8_t Pos, IPortState State) {
    // Convert from interface enum to driver enum
    PortState driverState;
    switch (State) {
        case IPortState::Off:
            driverState = Off;
            break;
        case IPortState::On:
            driverState = On;
            break;
        case IPortState::PWM:
            driverState = PWM;
            break;
        case IPortState::Group:
            driverState = Group;
            break;
        default:
            driverState = Off; // Default case
            break;
    }
    return driver.setOutput(Pos, driverState);
}

int LedPCA9634::setOutputArray(IPortState Val) {
    // Convert from interface enum to driver enum
    PortState driverState;
    switch (Val) {
        case IPortState::Off:
            driverState = Off;
            break;
        case IPortState::On:
            driverState = On;
            break;
        case IPortState::PWM:
            driverState = PWM;
            break;
        case IPortState::Group:
            driverState = Group;
            break;
        default:
            driverState = Off; // Default case
            break;
    }
    return driver.setOutputArray(driverState);
}