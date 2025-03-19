#pragma once

// This file contains includes and settings for testing the Kestrel driver with real code
// It ensures we properly redirect hardware calls to mocks while avoiding redefinition errors

// Tell the compiler we're in testing mode - this can be used in #ifdef blocks
#define TESTING

// First define all the hardware classes as empty forward declarations
// This prevents the real implementations from being compiled
class Sensor;
class PCAL9535A;
class PCA9634;
class MCP79412;
class SFE_UBLOX_GNSS;
class PAC1934;
class VEML3328;
class Adafruit_SHT4x;
class MXC6655;
class BMA456;

// Now include our mock implementations
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
#include "MockArduino.h"
#include "MockWireDeclare.h"

// Define any constants that might be needed by Kestrel.h
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

// Arduino constants
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D22 22
#define D23 23

// Define additional constants that Kestrel.h might need
// If there are specific enum values or constants that Kestrel.h expects,
// add them here