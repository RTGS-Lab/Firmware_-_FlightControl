#ifndef TESTING  // If not in testing mode
#include "Particle.h"  // Include the Particle IoT platform library for production
#else
#include "MockParticle.h"  // Include mock Particle library for testing environment
#endif

/*
 * Project FlightControl_Demo
 * Description: Core controller for Flight data logger
 * Author: Bobby Schulz
 * Date: 06/14/2022
 * © 2023 Regents of the University of Minnesota. All rights reserved.
 */

// #define RAPID_START  // Uncomment to skip waiting for remote connection on startup
// Function declarations
void setup();                                  // Initialize hardware and firmware
void loop();                                   // Main program loop
void logEvents(uint8_t type, uint8_t destination);  // Log data based on specified type and destination
String getErrorString();                       // Get error information from all sensors
String getDataString();                        // Get measurement data from all sensors
String getDiagnosticString(uint8_t level);     // Get diagnostic data at specified detail level
String getMetadataString();                    // Get system metadata
String initSensors();                          // Initialize all sensors
void quickTalonShutdown();                     // Quick shutdown routine for Talon boards
bool serialConnected();                        // Check if serial monitor is connected
void systemConfig();                           // System configuration menu via serial
int sleepSensors();                            // Put sensors into sleep mode
int wakeSensors();                             // Wake sensors from sleep mode
int detectTalons(String dummyStr);             // Auto-detect connected Talon boards
int detectSensors(String dummyStr);            // Auto-detect sensors connected to Talons
int setNodeID(String nodeID);                  // Set node identifier
int takeSample(String dummy);                  // Manually trigger sensor sampling
int commandExe(String command);                // Execute system commands
int systemRestart(String resetType);           // Restart the system
int configurePowerSave(int desiredPowerSaveMode);  // Configure power saving mode

#define WAIT_GPS false  // Configuration to wait for GPS fix before continuing
#define USE_CELL        // System attempts to connect to cellular network

// Hardware interface libraries
#include <AuxTalon.h>     // Auxiliary Talon board for interfacing with sensors
#include <PCAL9535A.h>    // I/O Expander chip driver
#include <Sensor.h>       // Base sensor class
#include <Talon.h>        // Base Talon interface board class

// Sensor-specific libraries
#include <BaroVue10.h>    // Barometric pressure sensor
#include <Kestrel.h>      // Main logger/controller board
#include <KestrelFileHandler.h>  // File handling for the Kestrel
#include <Haar.h>         // Temperature/pressur/relative humidity sensor
#include <Hedorah.h>      // CO2 sensor
#include <Aleppo.h>       // Differential Temperature sensor
#include <T9602.h>        // Temperature/Relative Humidity sensor
#include <SO421.h>        // Oxygen sensor
#include <SP421.h>        // Solar radiation sensor
#include <TEROS11.h>      // Soil moisture sensor
#include <ATMOS22.h>      // Ultrasonic Wind/Weather station sensor
#include <TDR315H.h>      // Time-domain reflectometry soil sensor
#include <Li710.h>        // Evapotranspiration sensor

// Talon variants
#include <I2CTalon.h>     // I2C protocol Talon board
#include <SDI12Talon.h>   // SDI-12 protocol Talon board
#include <Gonk.h>         // Battery management board

// Standard libraries
#include <vector>         // Dynamic arrays
#include <memory>         // Smart pointers

// Firmware versioning
const String firmwareVersion = "2.9.11";   // Current firmware version
const String schemaVersion = "2.2.9";      // Data schema version for compatibility

// System timing parameters
const unsigned long maxConnectTime = 180000;          // Wait up to 180 seconds for systems to connect 
const unsigned long indicatorTimeout = 60000;         // Wait for up to 1 minute with indicator lights on
const uint64_t balancedDiagnosticPeriod = 3600000;    // Report diagnostics once an hour (1 hour in ms)
int powerSaveMode = 0;                                // Default to 0, update when configure power save mode is called 

// Core system objects
Kestrel logger(true);                  // Initialize logger with diagnostics enabled
KestrelFileHandler fileSys(logger);    // File system handler using the logger
Gonk battery(5);                       // Battery management, manually set to port 5 

// Talon interface boards - initialized with default port (0) to be auto-detected later
AuxTalon aux(0, 0x14);                 // AUX talon with hardware v1.4
I2CTalon i2c(0, 0x21);                 // I2C talon with hardware v2.1
SDI12Talon sdi12(0, 0x14);             // SDI-12 talon with hardware v1.4

// I/O expander chips
PCAL9535A ioAlpha(0x20);               // I/O expander at I2C address 0x20
PCAL9535A ioBeta(0x21);                // I/O expander at I2C address 0x21

// Global variables
String globalNodeID = "";              // Store current node ID

// Talon configuration
const uint8_t numTalons = 3;           // Number must match the number of objects defined in `talonsToTest` array

// Talon arrays for device management
Talon* talons[Kestrel::numTalonPorts];    // Array to store all detected Talons, indexed by port number
Talon* talonsToTest[numTalons] = {        // Array of Talons to test during auto-detection
	&aux,                                 // Auxiliary Talon
	&i2c,                                 // I2C protocol Talon
	&sdi12                                // SDI-12 protocol Talon
};

// Defines different logging modes
namespace LogModes {
	constexpr uint8_t STANDARD = 0;      // Standard logging (balanced between data and diagnostics)
	constexpr uint8_t PERFORMANCE = 1;    // Maximum performance logging (high frequency data)
	constexpr uint8_t BALANCED = 2;       // Balanced logging (optimized for battery life)
	constexpr uint8_t NO_LOCAL = 3;       // Same as standard log, but no attempt to log to SD card
};

/////////////////////////// BEGIN USER CONFIG ////////////////////////
// Particle device configuration
//PRODUCT_ID(18596)      // Configured based on the target product, comment out if device has no product
PRODUCT_VERSION(32)      // Firmware version number for the Particle ecosystem

// Logging and power configuration
const int backhaulCount = 4;                               // Number of log events before data is sent to the cloud
const unsigned long logPeriod = 300;                       // Seconds to wait between logging events (5 minutes)
int desiredPowerSaveMode = PowerSaveModes::LOW_POWER;      // Power saving mode: PERFORMANCE, BALANCED, LOW_POWER, ULTRA_LOW_POWER 
int loggingMode = LogModes::STANDARD;                      // Logging mode: STANDARD, PERFORMANCE, BALANCED, NO_LOCAL

// Sensor initialization - ports set to 0 for auto-detection
// Environmental sensors
Haar haar(0, 0, 0x20);                    // Temperature/humidity sensor with hardware v2.0
// Haar haar1(0, 0, 0x20);                // Additional temperature/humidity sensor (commented out)
// Haar haar2(0, 0, 0x20);                // Additional temperature/humidity sensor (commented out)
SO421 apogeeO2(sdi12, 0, 0);              // Oxygen sensor connected to SDI12 Talon
SP421 apogeeSolar(sdi12, 0, 0);           // Solar radiation sensor connected to SDI12 Talon
// TEROS11 soil(sdi12, 0, 0);             // Soil moisture sensor (commented out)

// Soil sensors
TDR315H soil1(sdi12, 0, 0);               // Time domain reflectometry soil sensor #1
TDR315H soil2(sdi12, 0, 0);               // Time domain reflectometry soil sensor #2
TDR315H soil3(sdi12, 0, 0);               // Time domain reflectometry soil sensor #3

// Atmospheric sensors
Hedorah gas(0, 0, 0x10);                  // CO2 sensor with hardware v1.0
// T9602 humidity(0, 0, 0x00);            // Telair humidity sensor (commented out)
LI710 et(sdi12, 0, 0);                    // Evapotranspiration sensor connected to SDI12 Talon
BaroVue10 campPressure(sdi12, 0, 0x00);   // Barometric pressure sensor connected to SDI12 Talon

// Sensor array configuration
const uint8_t numSensors = 7;              // Number must match the number of objects defined in `sensors` array

// Array of all sensors in the system
Sensor* const sensors[numSensors] = {
	&fileSys,       // File system handler
	&aux,           // Auxiliary Talon
	&i2c,           // I2C Talon
	&sdi12,         // SDI-12 Talon
	&battery,       // Battery management
	&logger,        // Kestrel logger
	&et             // Evapotranspiration sensor
	// Commented out sensors can be enabled as needed
	// &haar,       // Temperature/humidity sensor
	// &soil1,      // Soil sensor #1
	// &apogeeSolar, // Solar radiation sensor
	
	// &soil2,      // Soil sensor #2
	// &soil3,      // Soil sensor #3
	// &gas,        // CO2 sensor
	// &apogeeO2,   // Oxygen sensor
};
/////////////////////////// END USER CONFIG /////////////////////////////////

// Pin definitions for various hardware interfaces

// Main board pins
namespace PinsIO {                       // For Kestrel v1.1 main board
	constexpr uint16_t VUSB = 5;         // USB voltage detection pin
}

// First I/O expander pins (ioAlpha)
namespace PinsIOAlpha {
	constexpr uint16_t I2C_EXT_EN = 10;  // External I2C bus enable pin
	constexpr uint16_t SD_CD = 8;        // SD card detect pin
	constexpr uint16_t SD_EN = 12;       // SD card power enable pin
	constexpr uint16_t AUX_EN = 15;      // Auxiliary power enable pin
	constexpr uint16_t CE = 11;          // Chip enable pin
	constexpr uint16_t LED_EN = 13;      // LED indicators enable pin
}

// Second I/O expander pins (ioBeta)
namespace PinsIOBeta {                   // For Kestrel v1.1 
	// Talon port selection pins
	constexpr uint16_t SEL1 = 0;         // Port 1 select pin
	constexpr uint16_t SEL2 = 4;         // Port 2 select pin
	constexpr uint16_t SEL3 = 8;         // Port 3 select pin
	constexpr uint16_t SEL4 = 12;        // Port 4 select pin
	
	// I2C enable pins for each port
	constexpr uint16_t I2C_EN1 = 1;      // Port 1 I2C enable
	constexpr uint16_t I2C_EN2 = 5;      // Port 2 I2C enable
	constexpr uint16_t I2C_EN3 = 9;      // Port 3 I2C enable
	constexpr uint16_t I2C_EN4 = 13;     // Port 4 I2C enable
	
	// Power enable pins for each port
	constexpr uint16_t EN1 = 3;          // Port 1 power enable
	constexpr uint16_t EN2 = 7;          // Port 2 power enable
	constexpr uint16_t EN3 = 11;         // Port 3 power enable
	constexpr uint16_t EN4 = 15;         // Port 4 power enable
	
	// Fault detection pins for each port
	constexpr uint16_t FAULT1 = 2;       // Port 1 fault detection
	constexpr uint16_t FAULT2 = 6;       // Port 2 fault detection
	constexpr uint16_t FAULT3 = 10;      // Port 3 fault detection
	constexpr uint16_t FAULT4 = 14;      // Port 4 fault detection
}

// Particle system operation mode configuration
// SYSTEM_MODE(MANUAL);        // User must call Particle.process() to stay connected to cellular after connecting (not recommended)
SYSTEM_MODE(SEMI_AUTOMATIC);   // Particle will wait until told to connect to cellular, but try to stay connected once connected
// SYSTEM_MODE(AUTOMATIC);     // Particle automatically tries to connect to cellular, once connected, user code starts running

// Thread configuration
SYSTEM_THREAD(ENABLED);        // Network processing runs on a different thread than user loop code (recommended)
// SYSTEM_THREAD(DISABLED);    // Network processing runs in the same thread as user code

// Forward declarations for functions used before they're defined
int detectTalons(String dummyStr = "");   // Auto-detect connected Talon boards
int detectSensors(String dummyStr = "");  // Auto-detect sensors connected to Talons

// Global string buffers for storing different types of messages
String diagnostic = "";  // Storage for diagnostic information
String errors = "";      // Storage for error messages
String metadata = "";    // Storage for system metadata
String data = "";        // Storage for sensor data

// Main system initialization function - called once at startup
void setup() {
	// Configure power saving mode for the entire system based on user configuration
	configurePowerSave(desiredPowerSaveMode); //Setup power mode of the system (Talons and Sensors)
	
	// Enable access to reset reason information for diagnostics
	System.enableFeature(FEATURE_RESET_INFO); //Allows for Particle to see reason for last reset using System.resetReason();
	
	// Handle unexpected reset scenarios
	if(System.resetReason() != RESET_REASON_POWER_DOWN) {
		//DEBUG! Set safe mode 
		Particle.connect(); //DEBUG! //If reset not caused by power switch, assume something bad happened, just connect to particle straight away
	}
	//////////// MANUAL POSITIONING //////////////////
  	// talons[aux.getTalonPort()] = &aux; //Place talon objects at coresponding positions in array
	// talons[aux1.getTalonPort()] = &aux1;
	
	// Record start time for connection timeout calculations
	time_t startTime = millis();
	
	// Register cloud functions that can be called remotely
	Particle.function("nodeID", setNodeID);          // Function to set the node identifier
	Particle.function("findSensors", detectSensors); // Function to detect connected sensors
	Particle.function("findTalons", detectTalons);   // Function to detect connected Talon boards
	Particle.function("systemRestart", systemRestart); // Function to remotely reboot the system
	Particle.function("takeSample", takeSample);     // Function to trigger a sensor data collection
	Particle.function("commandExe", commandExe);     // Function to execute various system commands
	// Initialize serial communication at 1Mbps for debugging
	Serial.begin(1000000); 
	waitFor(serialConnected, 10000); //DEBUG! Wait until serial starts sending or 10 seconds 
	
	// Output reset reason for debugging
	Serial.print("RESET CAUSE: "); //DEBUG!
	Serial.println(System.resetReason()); //DEBUG!
	
	// Error tracking flags
	bool hasCriticalError = false; // For critical system errors
	bool hasError = false;         // For non-critical issues
	
	// Initialize the logger system
	// logger.begin(Time.now(), hasCriticalError, hasError); //Needs to be called the first time with Particle time since I2C not yet initialized 
	logger.begin(0, hasCriticalError, hasError); //Called with 0 since time collection system has not been initialized 
	// Set all indicator lights to initialization mode
	logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::INIT);
	
	// Check if a battery is connected to the system
	bool batState = logger.testForBat(); //Check if a battery is connected
	
	// Configure I2C bus connections for battery monitoring
	logger.enableI2C_OB(false);              // Disable onboard I2C
	logger.enableI2C_External(true);         //Connect to Gonk I2C port
	logger.enableI2C_Global(true);           // Enable global I2C bus
	
	// Set battery indicator LEDs based on battery connection status
	if(batState) battery.setIndicatorState(GonkIndicatorMode::SOLID); //Turn on charge indication LEDs during setup 
	else battery.setIndicatorState(GonkIndicatorMode::BLINKING); //If battery not switched on, set to blinking 
	
	// Initialize file system (passing 0 for time since time not yet established)
	fileSys.begin(0, hasCriticalError, hasError); //Initialzie, but do not attempt backhaul
	
	// Handle any critical errors that occurred during initialization
	if(hasCriticalError) {
		Serial.println(getErrorString()); //Report current error codes
		logger.setIndicatorState(IndicatorLight::STAT,IndicatorMode::ERROR); //Display error state if critical error is reported 
	}
	else logger.setIndicatorState(IndicatorLight::STAT,IndicatorMode::PASS); //If no critical fault, switch STAT off
	
	// Initialize battery management system
	battery.begin(0, hasCriticalError, hasError); //Init final CORE element
	//   I2C_OnBoardEn(true); 	
	// Wire.setClock(400000); //Confirm operation in fast mode
	// Wire.begin();
	
	// Switch I2C bus to internal connections for I/O expander communication
	logger.enableI2C_Global(false); //Connect to internal bus
	logger.enableI2C_OB(true);      // Enable onboard I2C
	
	// Initialize the I/O expander chips
	ioAlpha.begin(); //RESTORE - First I/O expander
	ioBeta.begin();  //RESTORE - Second I/O expander
	
	// ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT); //DEBUG
	// ioBeta.digitalWrite(PinsIOBeta::SEL2, LOW); //DEBUG
	
	// Configure and enable the LED indicators
	ioAlpha.pinMode(PinsIOAlpha::LED_EN, OUTPUT);
	ioAlpha.digitalWrite(PinsIOAlpha::LED_EN, LOW); //Turn on LED indicators (active low)
	// logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::IDLE);
	// waitFor(serialConnected, 10000); //DEBUG! Wait until serial starts sending or 10 seconds
	
	// Check if someone is connected via serial for command mode
	if(Serial.available()) {
		//COMMAND MODE!
		logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::COMMAND); // Set indicators to command mode
		systemConfig();  // Enter the system configuration menu
	}
	
	// Set indicators to waiting mode while establishing connections
	logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::WAITING);
	
	// Attempt to connect to the Particle cloud
	Particle.connect(); //Once passed attempted serial connect, try to connect to particle 

	////////// ADD INTERRUPTS!
	// for(int i = 1; i <= Kestrel::numTalonPorts; i++) { //Iterate over ALL ports
	// 	logger.enablePower(i, true); //Turn on all power by default
	// 	logger.enablePower(i, false); //Toggle power to reset
	// 	logger.enablePower(i, true); 
	// 	logger.enableData(i, false); //Turn off all data by default
	// }

	// Auto-detect and initialize connected hardware
	detectTalons();   // Detect and configure Talon interface boards
	detectSensors();  // Detect and configure sensors connected to Talons

	// I2C_OnBoardEn(false);	
	// I2C_GlobalEn(true);

	// bool hasCriticalError = false;
	// bool hasError = false;
	// logger.enableData(4, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	// String initDiagnostic = aux.begin(Time.now(), hasCriticalError, hasError);
	// Update GPS location data
	logger.updateLocation(true); //Force GPS update (hopfully connected)
	
	// Initialize all sensors and get diagnostic information
	String initDiagnostic = initSensors();
	
	// Print diagnostic information to serial for debugging
	Serial.print("DIAGNOSTIC: ");
	Serial.println(initDiagnostic);
	
	// Handle the initial diagnostics based on logging mode
	if(loggingMode == LogModes::NO_LOCAL) {
		// For cloud-only logging mode
		fileSys.writeToFRAM(initDiagnostic, DataType::Diagnostic, DestCodes::Particle);
		logEvents(4, DestCodes::Particle); //Grab data log with metadata, no diagnostics 
	}
	else {
		// For standard logging to both SD and cloud
		fileSys.writeToFRAM(initDiagnostic, DataType::Diagnostic, DestCodes::Both);
		logEvents(4, DestCodes::Both); //Grab data log with metadata, no diagnostics 
	}
	// fileSys.writeToParticle(initDiagnostic, "diagnostic"); 
	// // logger.enableSD(true);
	// fileSys.writeToSD(initDiagnostic, "Dummy.txt");

	#ifndef RAPID_START  //Only do this if not rapid starting
	// Wait for connections to be established or timeout
	while((!Particle.connected() || logger.gps.getFixType() == 0) && (millis() - startTime) < maxConnectTime) { //Wait while at least one of the remote systems is not connected 
		// Check Particle cloud connection status
		if(Particle.connected()) {
			// Set cellular indicator to success if connected
			logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::PASS); //If cell is connected, set to PASS state
			
			// Break out if we're not waiting for GPS
			if(WAIT_GPS == false) break; //If not told to wait for GPS, break out after cell is connected 
		}
		
		// Check GPS status
		if(logger.gps.getTimeValid() == true) {
			// If we have a 2D or 3D fix and it's valid
			if(logger.gps.getFixType() >= 2 && logger.gps.getFixType() <= 4 && logger.gps.getGnssFixOk()) { //If you get a 2D fix or better, pass GPS 
				logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PASS); 
			}
			else {
				// If we have time data but no fix, set to preliminary pass
				logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PREPASS); //If time is good, set preliminary pass only
			}
		}
		
		Serial.println("Wait for cell connect..."); //DEBUG!
		delay(5000); //Wait 5 seconds between each check to not lock up the process //DEBUG!
	}
	#endif
	// Final connection status update
	if(Particle.connected()) {
		// Set cellular indicator to pass if connected
		logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::PASS); //Catches connection of cell is second device to connect
	}
	else {
		// Set cellular indicator to error if not connected
		logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::ERROR); //If cell still not connected, display error
		// Particle.disconnect(); //DEBUG!
	}
	
	// Final GPS status update
	if(logger.gps.getFixType() >= 2 && logger.gps.getFixType() <= 4 && logger.gps.getGnssFixOk()) { //Make fix report is in range and fix is OK
		// Set GPS indicator to pass if we have a valid fix
		logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PASS); //Catches connection of GPS is second device to connect
	}
	else {
		// Set GPS indicator to error if no fix or invalid
		logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::ERROR); //If GPS fails to connect after period, set back to error
	}
	
	// Attempt to send any logs stored in FRAM
	fileSys.tryBackhaul(); //See if we can backhaul any unsent logs

	// fileSys.writeToFRAM(getDiagnosticString(1), DataType::Diagnostic, DestCodes::Both); //DEBUG!
	// logEvents(3); //Grab data log with metadata //DEBUG!
	
	// Send all accumulated data to the cloud immediately
	fileSys.dumpFRAM(); //Backhaul this data right away
	
	// Particle.publish("diagnostic", initDiagnostic);
	// logger.enableData(3, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	// aux1.begin(Time.now(), hasCriticalError, hasError);
  	//FIX! RESPOND TO ERROR RESULTS! 
}

// Main program loop - executes repeatedly
void loop() {
  // aux.sleep(false);
  static int count = 1; //Keep track of number of cycles since startup
  
	// Wake up all system components
	logger.wake();    // Wake up logger system from sleep
	fileSys.wake();   // Wake up file handling system
	wakeSensors();    // Wake up all sensors
	
	// Turn off indicator LEDs after the initial time period
	if(System.millis() > indicatorTimeout) {
		// Disable all status indicator LEDs after timeout period
		logger.setIndicatorState(IndicatorLight::ALL, IndicatorMode::NONE); //Turn LED indicators off if it has been longer than timeout since startup (use system.millis() which does not rollover)
		
		// Configure I2C bus for battery communication
		logger.enableI2C_OB(false);
		logger.enableI2C_External(true); //Connect to Gonk I2C port
		logger.enableI2C_Global(true);
		
		// Set battery indicator to push button control mode
		battery.setIndicatorState(GonkIndicatorMode::PUSH_BUTTON); //Turn off indicator lights on battery, return to push button control
		
		// Disconnect from external I2C
		logger.enableI2C_External(false); //Turn off external I2C
	}
	
	// Wait until the logging period has elapsed
	bool alarm = logger.waitUntilTimerDone(); //Wait until the timer period has finished  //REPLACE FOR NON-SLEEP
	
	// if(alarm) Serial.println("RTC Wakeup"); //DEBUG!
	// else Serial.println("Timeout Wakeup"); //DEBUG!
	// Serial.print("RAM, Start Log Events: "); //DEBUG!
	// Serial.println(System.freeMemory()); //DEBUG!
	// Start the timer for the next logging event
	logger.startTimer(logPeriod); //Start timer as soon done reading sensors //REPLACE FOR NON-SLEEP
	
	// Handle data logging based on configured logging mode
	switch(loggingMode) {
		static uint64_t lastDiagnostic = System.millis(); // Timestamp for periodic diagnostics
		
		case (LogModes::PERFORMANCE):
			// High performance mode - only logs data (no diagnostics or metadata)
			logEvents(6, DestCodes::Both); // Log only data (fastest method)
			break;
			
		case (LogModes::STANDARD):
			// Standard mode with varying log detail based on cycle count
			if((count % 16) == 0) logEvents(3, DestCodes::Both);      // Every 16th cycle: full log with metadata
			else if((count % 8) == 0) logEvents(2, DestCodes::Both);  // Every 8th cycle: data with diagnostics
			else if((count % 1) == 0) logEvents(1, DestCodes::Both);  // Every cycle: data with minimal diagnostics
			break;
			
		case (LogModes::BALANCED):
			// Balanced mode - efficient logging with periodic full diagnostics
			logEvents(7, DestCodes::Both); // Log data and errors only
			
			// Add full diagnostic and metadata logs periodically
			if((System.millis() - lastDiagnostic) > balancedDiagnosticPeriod) {
				logEvents(3, DestCodes::Both); //Do a full diagnostic and metadata report once an hour
				lastDiagnostic = System.millis();
			}
			break;
			
		case (LogModes::NO_LOCAL):
			// Cloud-only mode (no SD card logging)
			if((count % 10) == 0) logEvents(3, DestCodes::Particle);      // Every 10th cycle: full log
			else if((count % 5) == 0) logEvents(2, DestCodes::Particle);  // Every 5th cycle: data with diagnostics
			else if((count % 1) == 0) logEvents(1, DestCodes::Particle);  // Every cycle: data with minimal diagnostics
			break;
			
		default:
			// Fallback for undefined logging modes
			logEvents(1, DestCodes::Both); //If unknown configuration, use general call 
			// break;
	}
	// Debug output
	// Serial.print("RAM, End Log Events: "); //DEBUG!
	// Serial.println(System.freeMemory()); //DEBUG!
	Serial.println("Log Done"); //DEBUG!
	
	// Feed the watchdog timer to prevent system reset
	Serial.print("WDT Status: "); //DEBUG!
	Serial.println(logger.feedWDT()); 
	
	// Put all sensors into sleep mode to save power
	sleepSensors();
	
	// Alternative ways to publish data for debugging:
	// Particle.publish("diagnostic", diagnostic);
	// Particle.publish("error", errors);
	// Particle.publish("data", data);
	// Particle.publish("metadata", metadata);

	// Serial.print("DIAGNOSTIC: ");
	// Serial.println(diagnostic);
	// Serial.print("ERROR: ");
	// Serial.println(errors);
	// Serial.print("DATA: ");
	// Serial.println(data);
	// Serial.print("METADATA: ");
	// Serial.println(metadata);

	// logger.enableI2C_OB(true);
	// logger.enableI2C_Global(false);
	// fileSys.writeToFRAM(diagnostic, "diagnostic", DestCodes::Particle);

	// Periodically send accumulated data to the cloud
	if((count % backhaulCount) == 0) {
		Serial.println("BACKHAUL"); //DEBUG!
		
		// For lower power modes, connect to Particle only when needed
		if(powerSaveMode >= PowerSaveModes::LOW_POWER) {
			Particle.connect();
			waitFor(Particle.connected, 300000); //Wait up to 5 minutes to connect if using low power modes
		}
		
		// Synchronize system time with cloud time
		logger.syncTime();
		
		// Send accumulated data
		fileSys.dumpFRAM(); //dump FRAM every Nth log
	}
	
	// Increment cycle counter
	count++;
	
	// Put systems to sleep to save power
	fileSys.sleep(); //Wait to sleep until after backhaul attempt
	logger.sleep(); //Put system into sleep mode

	// Alternative deep sleep configuration:
	// SystemSleepConfiguration config;
	// config.mode(SystemSleepMode::STOP)
    //   .network(NETWORK_INTERFACE_CELLULAR)
    //   .flag(SystemSleepFlag::WAIT_CLOUD)
    //   .duration(2min);
	// System.sleep(config);
}

/**
 * @brief Logs system data based on specified type and destination
 * 
 * @param type Controls what combination of data to log:
 *             0: Errors only
 *             1: Data with minimal diagnostics
 *             2: Data with more detailed diagnostics
 *             3: Data with diagnostics and metadata
 *             4: Data with metadata (no diagnostics)
 *             5: SD-only logging with diagnostics
 *             6: Data only (highest performance)
 *             7: Data and errors only (balanced mode)
 * @param destination Where to send the data (Particle cloud, SD card, or both)
 */
void logEvents(uint8_t type, uint8_t destination)
{
	// Clear previous string buffers
	// String diagnostic = "";
	// String errors = "";
	// String metadata = "";
	// String data = "";
	diagnostic = "";
	errors = "";
	metadata = "";
	data = "";
	Serial.print("LOG: "); //DEBUG!
	Serial.println(type); 
	if(type == 0) { //Grab errors only
		// data = getDataString();
		// diagnostic = getDiagnosticString(4); //DEBUG! RESTORE
		errors = getErrorString(); //Get errors last to wait for error codes to be updated //DEBUG! RESTORE
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		Serial.println(errors); //DEBUG!
		// Serial.println(data); //DEBUG!
		// Serial.println(diagnostic); //DEBUG!

		if(errors.equals("") == false) {
			// Serial.println("Write Errors to FRAM"); //DEBUG!
			fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		}
		// fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
		// fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
	}
	if(type == 1) {
		data = getDataString();
		diagnostic = getDiagnosticString(4); //DEBUG! RESTORE
		errors = getErrorString(); //Get errors last to wait for error codes to be updated //DEBUG! RESTORE
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		Serial.println(errors); //DEBUG!
		Serial.println(data); //DEBUG!
		Serial.println(diagnostic); //DEBUG!

		if(errors.equals("") == false) {
			// Serial.println("Write Errors to FRAM"); //DEBUG!
			fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		}
		fileSys.writeToFRAM(data, DataType::Data, destination);
		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, destination);
	}
	else if(type == 2) {
		data = getDataString();
		diagnostic = getDiagnosticString(3);
		errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, destination);
		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, destination);
	}
	else if(type == 3) {
		data = getDataString();
		diagnostic = getDiagnosticString(2);
		metadata = getMetadataString();
		errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, destination);
		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, destination);
		fileSys.writeToFRAM(metadata, DataType::Metadata, destination);
	}
	else if(type == 4) { //To be used on startup, don't grab diagnostics since init already got them
		data = getDataString();
		// diagnostic = getDiagnosticString(2);
		metadata = getMetadataString();
		errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		Serial.println(errors); //DEBUG!
		Serial.println(data); //DEBUG
		Serial.println(metadata); //DEBUG!
		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, destination);
		// fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
		fileSys.writeToFRAM(metadata, DataType::Metadata, destination);
	}
	else if(type == 5) { //To be used on startup, don't grab diagnostics since init already got them
		data = getDataString();
		diagnostic = getDiagnosticString(5);
		// metadata = getMetadataString();
		errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		Serial.println(errors); //DEBUG!
		Serial.println(data); //DEBUG
		// Serial.println(metadata); //DEBUG!
		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::SD); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, DestCodes::SD);
		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::SD);
		// fileSys.writeToFRAM(metadata, DataType::Metadata, DestCodes::Both);
	}
	else if(type == 6) { //Log ONLY data - fastest method
		data = getDataString();
		// diagnostic = getDiagnosticString(5);
		// metadata = getMetadataString();
		// errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		// Serial.println(errors); //DEBUG!
		// Serial.println(data); //DEBUG
		// Serial.println(metadata); //DEBUG!
		// if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::SD); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, destination);
		// fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::SD);
		// fileSys.writeToFRAM(metadata, DataType::Metadata, DestCodes::Both);
	}
	else if(type == 7) { //Log data and error if there
		data = getDataString();
		// diagnostic = getDiagnosticString(5);
		// metadata = getMetadataString();
		errors = getErrorString();
		// logger.enableI2C_OB(true);
		// logger.enableI2C_Global(false);
		// Serial.println(errors); //DEBUG!
		// Serial.println(data); //DEBUG
		// Serial.println(metadata); //DEBUG!
		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, destination); //Write value out only if errors are reported 
		fileSys.writeToFRAM(data, DataType::Data, destination);
		// fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::SD);
		// fileSys.writeToFRAM(metadata, DataType::Metadata, DestCodes::Both);
	}
	// switch(type) {
	// 	case 1: //Standard, short interval, log
			
	// 		data = getDataString();
	// 		diagnostic = getDiagnosticString(4); //DEBUG! RESTORE
	// 		errors = getErrorString(); //Get errors last to wait for error codes to be updated //DEBUG! RESTORE
	// 		// logger.enableI2C_OB(true);
	// 		// logger.enableI2C_Global(false);
	// 		Serial.println(errors); //DEBUG!
	// 		Serial.println(data); //DEBUG!
	// 		Serial.println(diagnostic); //DEBUG!

	// 		if(errors.equals("") == false) {
	// 			// Serial.println("Write Errors to FRAM"); //DEBUG!
	// 			fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
	// 		}
	// 		fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
	// 		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
	// 	break;

	// 	case 2: //Low period log with diagnostics
	// 		data = getDataString();
	// 		diagnostic = getDiagnosticString(3);
	// 		errors = getErrorString();
	// 		// logger.enableI2C_OB(true);
	// 		// logger.enableI2C_Global(false);
	// 		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
	// 		fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
	// 		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
	// 	break;

	// 	case 3: //Daily log event with increased diagnostics and metadata
	// 		data = getDataString();
	// 		diagnostic = getDiagnosticString(2);
	// 		metadata = getMetadataString();
	// 		errors = getErrorString();
	// 		// logger.enableI2C_OB(true);
	// 		// logger.enableI2C_Global(false);
	// 		if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
	// 		fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
	// 		fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
	// 		fileSys.writeToFRAM(metadata, DataType::Metadata, DestCodes::Both);
	// 	break;

	// }
	// diagnostic* = (const char*)NULL;
	// data* = (const char*)NULL;
	// metadata* = (const char*)NULL;
	// errors* = (const char*)NULL;
	
}
/**
 * @brief Collects error information from all sensors into a JSON string
 * 
 * @return String JSON-formatted error information or empty string if no errors
 */
String getErrorString()
{
	unsigned long numErrors = 0; //Used to keep track of total errors across all devices 
	
	// Create JSON structure for error information
	String errors = "{\"Error\":{";
	errors = errors + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	errors = errors + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosAlt() + "," + logger.getPosTimeString() + "],";
	
	// Add device identification
	if(globalNodeID != "") errors = errors + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else errors = errors + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	
	// Add packet metadata
	errors = errors + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	errors = errors + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	errors = errors + "\"Devices\":[";
	
	// Collect errors from each sensor that has reported errors
	for(int i = 0; i < numSensors; i++) {
		if(sensors[i]->totalErrors() > 0) {
			numErrors = numErrors + sensors[i]->totalErrors(); //Increment the total error count
			if(!errors.endsWith("[")) errors = errors + ","; //Only append if not first entry
			errors = errors + "{" + sensors[i]->getErrors() + "}";
		}
	}
	
	// Close JSON structure
	errors = errors + "]}}"; //Close data
	
	Serial.print("Num Errors: "); //DEBUG!
	Serial.println(numErrors); 
	
	// Only return the error string if there are actual errors
	if(numErrors > 0) return errors;
	else return ""; //Return null string if no errors reported 
}

/**
 * @brief Collects measurement data from all sensors into a JSON string
 * 
 * @return String JSON-formatted sensor data
 */
String getDataString()
{
	// Create header section of the JSON structure
	String leader = "{\"Data\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosAlt() + "," + logger.getPosTimeString() + "],";
	
	// Add device identification
	if(globalNodeID != "") leader = leader + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else leader = leader + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	
	// Add packet metadata
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	leader = leader + "\"Devices\":[";
	
	// Define closing string for the JSON structure
	const String closer = "]}}";
	
	// Initialize output with the header
	String output = leader;

	uint8_t deviceCount = 0; //Used to keep track of how many devices have been appended 
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enablePower(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon, only if not core system and port is valid
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon, only if not core system and port is valid
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		bool dummy1;
		bool dummy2;
		
		if(sensors[i]->getTalonPort() > 0 && talons[sensors[i]->getTalonPort() - 1]) { //DEBUG! REPALCE!
			Serial.print("TALON CALL: "); //DEBUG!
			Serial.println(sensors[i]->getTalonPort());
			logger.configTalonSense(); //Setup to allow for current testing
			// talons[sensors[i]->getTalonPort() - 1]->begin(logger.getTime(), dummy1, dummy2); //DEBUG! Do only if talon is associated with sensor, and object exists 
			talons[sensors[i]->getTalonPort() - 1]->restart(); //DEBUG! Do only if talon is associated with sensor, and object exists 
			// logger.enableI2C_OB(false); //Return to isolation mode
			// logger.enableI2C_Global(true);
		}
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If not a Talon
			Serial.print("Device "); //DEBUG!
			Serial.print(i);
			Serial.println(" is a sensor");
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data ports to start for the given Talon
			// talons[sensors[i]->getTalonPort() - 1]->disablePowerAll(); //Turn off all power ports to start for the given Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn on data for the given port on the Talon
			// bool dummy1;
			// bool dummy2;
			// sensors[i]->begin(Time.now(), dummy1, dummy2); //DEBUG!
		}
		// delay(100); //DEBUG!
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		Serial.print("Data string from sensor "); //DEBUG!
		Serial.print(i);
		Serial.print(": ");
		String val = sensors[i]->getData(logger.getTime());
		Serial.println(val);
		if(!val.equals("")) {  //Only append if not empty string
			if(output.length() - output.lastIndexOf('\n') + val.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
				if(deviceCount > 0) output = output + ","; //Add preceeding comma if not the first entry
				output = output + "{" + val + "}"; //Append result 
				deviceCount++;
				// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
			}
			else {
				output = output + closer + "\n"; //End this packet
				output = output + leader + "{" + val + "}"; //Start a new packet and add new payload 
			}
		}
		// if(!val.equals("")) { //Only append if real result
		// 	if(deviceCount > 0) data = data + ","; //Preappend comma only if not first addition
		// 	data = data + "{" + val + "}";
		// 	deviceCount++;
		// 	// if(i + 1 < numSensors) metadata = metadata + ","; //Only append if not last entry
		// }
		Serial.print("Cumulative data string: "); //DEBUG!
		Serial.println(output); //DEBUG!
		// data = data + sensors[i]->getData(logger.getTime()); //DEBUG! REPLACE!
		// if(i + 1 < numSensors) data = data + ","; //Only append if not last entry
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) {
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), false); //Turn off power for the given port on the Talon //DEBUG!
		}
	}
	output = output + "]}}"; //Close data
	return output;
}

/**
 * @brief Collects diagnostic information from all sensors into a JSON string
 * 
 * @param level Level of diagnostic detail to collect (1-5):
 *              1: Minimal diagnostics
 *              2: Basic diagnostics with metadata (daily logs)
 *              3: Standard diagnostics (low period logs)
 *              4: Detailed diagnostics (standard logs)
 *              5: SD-only detailed diagnostics
 * @return String JSON-formatted diagnostic information
 */
String getDiagnosticString(uint8_t level)
{
	// Create header section of the JSON structure
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosAlt() + "," + logger.getPosTimeString() + "],";
	
	// Add device identification
	if(globalNodeID != "") leader = leader + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else leader = leader + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	
	// Add packet metadata
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Level\":" + String(level) + ",\"Devices\":["; //Concatonate number of sensors and level 
	
	// Define closing string for the JSON structure
	const String closer = "]}}";
	
	// Initialize output with the header
	String output = leader;

	uint8_t deviceCount = 0; //Used to keep track of how many devices have been appended 
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enablePower(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon, only if not core system and port is valid
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon, only if not core system and port is valid
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
			
		}

  		String diagnostic = sensors[i]->selfDiagnostic(level, logger.getTime());
		if(!diagnostic.equals("")) {  //Only append if not empty string
			if(output.length() - output.lastIndexOf('\n') + diagnostic.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
				if(deviceCount > 0) output = output + ","; //Add preceeding comma if not the first entry
				output = output + "{" + diagnostic + "}"; //Append result 
				deviceCount++;
				// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
			}
			else {
				output = output + closer + "\n"; //End this packet
				output = output + leader + "{" + diagnostic + "}"; //Start a new packet and add new payload 
			}
		}

		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) {
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), false); //Turn off power for the given port on the Talon //DEBUG!
		}
		
	}
	output = output + closer; //Close diagnostic
	return output;
}

/**
 * @brief Collects system metadata into a JSON string
 * 
 * Gathers metadata about the system including firmware version, schema version,
 * system configuration, and metadata from each sensor.
 * 
 * @return String JSON-formatted system metadata
 */
String getMetadataString()
{
	// Create header section of the JSON structure
	String leader = "{\"Metadata\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosAlt() + "," + logger.getPosTimeString() + "],";
	
	// Add device identification
	if(globalNodeID != "") leader = leader + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else leader = leader + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	
	// Add packet metadata
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	leader = leader + "\"Devices\":[";
	
	// Define closing string for the JSON structure
	const String closer = "]}}";
	
	// Initialize output with the header
	String output = leader;
	
	output = output + "{\"System\":{";
	// output = output + "\"DUMMY\":\"BLOODYMARYBLOODYMARYBLODDYMARY\",";
	output = output + "\"Schema\":\"" + schemaVersion + "\",";
	output = output + "\"Firm\":\"" + firmwareVersion + "\",";
	output = output + "\"OS\":\"" + System.version() + "\",";
	output = output + "\"ID\":\"" + System.deviceID() + "\",";
	output = output + "\"Update\":" + String(logPeriod) + ",";
	output = output + "\"Backhaul\":" + String(backhaulCount) + ",";
	output = output + "\"LogMode\":" + String(loggingMode) + ",";
	output = output + "\"Sleep\":" + String(powerSaveMode) + "}},";
	//FIX! Add support for device name 
	
	uint8_t deviceCount = 0; //Used to keep track of how many devices have been appended 
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port only if not core and port is valid
			// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
		}
		// logger.enablePower(sensors[i]->getTalon(), true); //Turn on power to port
		// logger.enableData(sensors[i]->getTalon(), true); //Turn on data to port
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		String val = sensors[i]->getMetadata();
		// metadata = metadata + sensors[i]->getMetadata();
		// if(!val.equals("")) { //Only append if real result
		// 	if(deviceCount > 0) metadata = metadata + ","; //Preappend comma only if not first addition
		// 	metadata = metadata + val;
		// 	deviceCount++;
		// 	// if(i + 1 < numSensors) metadata = metadata + ","; //Only append if not last entry
		// }
		if(!val.equals("")) {  //Only append if not empty string
			if(output.length() - output.lastIndexOf('\n') + val.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
				if(deviceCount > 0) output = output + ","; //Add preceeding comma if not the first entry
				output = output + "{" + val + "}"; //Append result 
				deviceCount++;
				// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
			}
			else {
				output = output + closer + "\n"; //End this packet
				output = output + leader + "{" + val + "}"; //Start a new packet and add new payload 
			}
		}
	}

	output = output + closer; //Close metadata
	return output;
}

/**
 * @brief Initializes all sensors and collects initial diagnostic information
 * 
 * This function handles the initialization of all connected sensors and captures
 * diagnostic information during the startup process. It also updates the system
 * status indicators based on sensor availability and errors.
 * 
 * @return String JSON-formatted diagnostic information from sensor initialization
 */
String initSensors()
{
	// Create header section of the JSON structure
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosAlt() + "," + logger.getPosTimeString() + "],";
	
	// Add device identification
	if(globalNodeID != "") leader = leader + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else leader = leader + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	
	// Add packet metadata
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Devices\":["; //Concatonate number of sensors and level 
	
	String closer = "]}}";
	String output = leader;
	bool reportCriticalError = false; //Used to keep track of the global status of the error indications for all sensors
	bool reportError = false;
	bool missingSensor = false;
	// output = output + "\"Devices\":[";
	uint8_t deviceCount = 0; //Used to keep track of how many devices have been appended 
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE && sensors[i]->getTalonPort() != 0) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port only if not core and the port is valid
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		bool dummy1;
		bool dummy2;
		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		logger.configTalonSense(); //Setup to allow for current testing
		// if(sensors[i]->getTalonPort() > 0 && talons[sensors[i]->getTalonPort() - 1]) talons[sensors[i]->getTalonPort() - 1]->begin(logger.getTime(), dummy1, dummy2); //DEBUG! Do only if talon is associated with sensor, and object exists //DEBUG! REPLACE!
		if(sensors[i]->getTalonPort() > 0 && talons[sensors[i]->getTalonPort() - 1]) talons[sensors[i]->getTalonPort() - 1]->restart(); //DEBUG! Do only if talon is associated with sensor, and object exists //DEBUG! REPLACE!
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
			
		}
		if(sensors[i]->getTalonPort() == 0 && sensors[i]->sensorInterface != BusType::CORE) {
			missingSensor = true; //Set flag if any sensors not assigned to Talon and not a core sensor
			Serial.print("Missing Sensor: "); //DEBUG!
			Serial.print(i);
			Serial.print("\t");
			Serial.println(sensors[i]->sensorInterface);
		}
		bool hasCriticalError = false;
		bool hasError = false;

  		String val;
		if(sensors[i]->getTalonPort() > 0 && sensors[i]->getSensorPort() > 0) val = sensors[i]->begin(logger.getTime(), hasCriticalError, hasError); //If detected sensor, run begin
		else if(sensors[i]->getTalonPort() > 0 && sensors[i]->getSensorPort() == 0 || sensors[i]->sensorInterface == BusType::CORE) val = sensors[i]->selfDiagnostic(2, logger.getTime()); //If sensor is a Talon or CORE type, run diagnostic, begin has already been run
		if(hasError) reportError = true; //Set if any of them throw an error
		if(hasCriticalError) reportCriticalError = true; //Set if any of them throw a critical error
		if(!val.equals("")) {  //Only append if not empty string
			if(output.length() - output.lastIndexOf('\n') + val.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
				if(deviceCount > 0) output = output + ","; //Add preceeding comma if not the first entry
				output = output + "{" + val + "}"; //Append result 
				deviceCount++;
				// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
			}
			else {
				output = output + closer + "\n"; //End this packet
				output = output + leader + "{" + val + "}"; //Start a new packet and add new payload 
			}
		}
		
	}
	if(missingSensor) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR);
	else logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS); //If no errors are reported, set to pass state
	//FIX! Replace!
	// if(reportCriticalError) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR_CRITICAL);
	// else if(reportError) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR); //Only set minimal error state if critical error is not thrown
	// else logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS); //If no errors are reported, set to pass state
	
	output = output + closer; //Close diagnostic
	return output;
}

/**
 * @brief Emergency shutdown of Talon boards via direct I2C commands
 * 
 * This function is a workaround for quickly shutting down Talon boards 
 * during the initialization sequence by directly accessing their I2C registers.
 * Used primarily when auto-detecting hardware to prevent power issues.
 */
void quickTalonShutdown()
{
	// Wire.beginTransmission(0x22); //Talk to I2C Talon
	// Wire.write(0x48); //Point to pullup/pulldown select reg
	// Wire.write(0xF0); //Set pins 1 - 4 as pulldown
	// Wire.endTransmission();

	// Wire.beginTransmission(0x22); //Talk to I2C Talon
	// Wire.write(0x46); //Point to pullup/pulldown enable reg
	// Wire.write(0x0F); //Enable pulldown on pins 1-4
	// Wire.endTransmission();

	//////////// DEBUG! /////////////
	//// SET SDI-12 TALON First to eliminiate issue with power being applied to Apogee port 
	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x02); //Point to output port
	Wire.write(0x00); //Set pints 1 - 8 low
	Wire.endTransmission();

	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x06); //Point to config port
	Wire.write(0x00); //Set pins 1 - 8 as output
	Wire.endTransmission();

	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x00); //Point to port reg
	// Wire.write(0xF0); //Set pints 1 - 4 low
	Wire.endTransmission();

	Wire.requestFrom(0x25, 1); 
	Wire.read(); //Read back current value

	Wire.beginTransmission(0x22); //Talk to I2C Talon
	Wire.write(0x06); //Point to config port
	Wire.write(0xF0); //Set pins 1 - 4 as output
	Wire.endTransmission();

	Wire.beginTransmission(0x22); //Talk to I2C Talon
	Wire.write(0x02); //Point to output port
	Wire.write(0xF0); //Set pints 1 - 4 low
	Wire.endTransmission();

	Wire.beginTransmission(0x22); //Talk to I2C Talon
	Wire.write(0x00); //Point to port reg
	// Wire.write(0xF0); //Set pints 1 - 4 low
	Wire.endTransmission();

	Wire.requestFrom(0x22, 1); 
	Wire.read(); //Read back current value
	/////////// END DEBUG! /////////////

	// Wire.beginTransmission(0x25); //Talk to SDI-12 Talon
	// Wire.write(0x48); //Point to pullup/pulldown select reg
	// Wire.write(0xF0); //Set pins 1 - 4 as pulldown
	// Wire.endTransmission();

	// Wire.beginTransmission(0x25); //Talk to SDI-12 Talon
	// Wire.write(0x46); //Point to pullup/pulldown enable reg
	// Wire.write(0x0F); //Enable pulldown on pins 1-4
	// Wire.endTransmission();

	
}

/**
 * @brief Checks if a serial monitor is connected
 * 
 * Used to detect if someone is trying to connect via serial at the beginning
 * of operation, which can trigger entering command mode for manual system configuration.
 * 
 * @return bool True if serial data is available, false otherwise
 */
bool serialConnected() 
{
	if(Serial.available() > 0) return true;
	else return false;
}

/**
 * @brief Provides a serial command interface for system configuration
 * 
 * This function implements a simple command-line interface for configuring
 * the system when connected via serial. It allows various maintenance
 * operations like erasing FRAM, zeroing accelerometer, etc.
 */
void systemConfig()
{
	Serial.println("HALT: Entered Command Mode - Here be Dragons"); //DEBUG!
	static int ReadLength = 0;   // Track current position in input buffer
  	String ReadString;           // String representation of command
	char ReadArray[25] = {0};    // Character buffer for input
	while(1) {
		if(Serial.available() > 0) {
			char Input = Serial.read();
			if(Input != '\r') { //Wait for return
				ReadArray[ReadLength] = Input;
				ReadLength++;
			}
			if(Input == '\r') {
				ReadString = String(ReadArray);
				ReadString.trim();
				memset(ReadArray, 0, sizeof(ReadArray));
				ReadLength = 0;

				Serial.print(">");
				Serial.println(ReadString); //Echo back to serial monitor

				if(ReadString.equalsIgnoreCase("Erase FRAM")) {
					fileSys.eraseFRAM();
					Serial.println("\tDone");
				}

				if(ReadString.equalsIgnoreCase("Set Accel Zero")) {
					logger.zeroAccel();
					Serial.println("\tDone");
				}

				if(ReadString.equalsIgnoreCase("Clear Accel Zero")) {
					logger.zeroAccel(true);
					Serial.println("\tDone");
				}

				if(ReadString.equalsIgnoreCase("Exit")) {
					return; //Exit the setup function
				}
			}
		}
	}
}

/**
 * @brief Puts all sensors into sleep mode
 * 
 * This function handles putting sensors and Talons into sleep mode to conserve power.
 * Only sensors that don't need to stay powered will be fully powered down, while
 * others will be put into their low-power sleep modes.
 * 
 * @return int 0 on success
 */
int sleepSensors()
{
	// Only enter sleep mode if not in performance mode
	if(powerSaveMode > PowerSaveModes::PERFORMANCE) { //Only turn off is power save requested 
		Serial.println("BEGIN SENSOR SLEEP"); //DEBUG!
		for(int s = 0; s < numSensors; s++) { //Iterate over all sensors objects
			//If not set to keep power on and Talon is assocated, power down sensor. Ignore if core device, we will handle these seperately 
			if(sensors[s]->keepPowered == false && sensors[s]->sensorInterface != BusType::CORE && sensors[s]->getTalonPort() > 0 && sensors[s]->getTalonPort() < numTalons) {
				Serial.print("Power Down Sensor "); //DEBUG!
				Serial.print(s + 1);
				Serial.print(",");
				Serial.println(sensors[s]->getTalonPort());
				talons[sensors[s]->getTalonPort() - 1]->enablePower(sensors[s]->getSensorPort(), false); //Turn off power for any sensor which does not need to be kept powered
			}
			else if(sensors[s]->sensorInterface != BusType::CORE && sensors[s]->getTalonPort() > 0 && sensors[s]->getTalonPort() < numTalons){ //If sensor has a position and is not core, but keepPowered is true, run sleep routine
				Serial.print("Sleep Sensor "); //DEBUG!
				Serial.println(s + 1);
				sensors[s]->sleep(); //If not powered down, run sleep protocol 
			}
			else if(sensors[s]->sensorInterface == BusType::CORE) {
				Serial.print("Sensor "); //DEBUG!
				Serial.print(s + 1);
				Serial.println(" is core, do nothing"); 
			}
			else {
				Serial.print("Sensor "); //DEBUG!
				Serial.print(s + 1);
				Serial.println(" not detected, do nothing"); 
			}
		}

		for(int t = 0; t < Kestrel::numTalonPorts; t++) { //Iterate over all talon objects
			if(talons[t] && talons[t]->keepPowered == false) { //If NO sensors on a given Talon require it to be kept powered, shut the whole thing down
				Serial.print("Power Down Talon "); //DEBUG!
				Serial.println(talons[t]->getTalonPort());
				logger.enablePower(talons[t]->getTalonPort(), false); //Turn off power to given port 
			}
			else if(!talons[t]) {
				Serial.print("Power Down Empty Port "); //DEBUG!
				Serial.println(t + 1);
				logger.enablePower(t + 1, false); //Turn off power to unused port
			}
		}
	}

	return 0; //DEBUG!
}

/**
 * @brief Wakes all sensors from sleep mode
 * 
 * Powers up all Talons and sensors, and calls their wake functions to
 * restore them to active operation after sleep.
 * 
 * @return int 0 on success
 */
int wakeSensors()
{
	// Configure I2C bus for external communication
	logger.enableI2C_Global(true); //Connect to external bus to talk to sensors/Talons
	logger.enableI2C_OB(false);
	logger.disableDataAll(); //Turn off all data to start
	for(int p = 1; p <= Kestrel::numTalonPorts; p++) logger.enablePower(p, true); //Turn power back on to all Kestrel ports
	for(int t = 0; t < Kestrel::numTalonPorts; t++) {
		if(talons[t] && talons[t]->getTalonPort() != 0) {
			logger.enableData(talons[t]->getTalonPort(), true); //Turn on data for given port
			talons[t]->restart(); //Restart all Talons, this turns on all ports it can
			logger.enableData(talons[t]->getTalonPort(), false); //Turn data back off for given port
		}
	}
	for(int s = 0; s < numSensors; s++) {
		if(sensors[s]->getTalonPort() != 0) {
			logger.enableData(sensors[s]->getTalonPort(), true); //Turn on data for given port
			sensors[s]->wake(); //Wake each sensor
			logger.enableData(sensors[s]->getTalonPort(), false); //Turn data back off for given port
		}
	}
	return 0; //DEBUG!
}

/**
 * @brief Auto-detects Talon boards connected to the system
 * 
 * Scans all Kestrel ports to detect which Talon boards are connected,
 * and initializes them. This allows the system to automatically adapt
 * to different hardware configurations.
 *
 * @param dummyStr Unused parameter (allows function to be called from Particle cloud)
 * @return int 0 on success
 */
int detectTalons(String dummyStr)
{
	////////////// AUTO TALON DETECTION ///////////////////////
	// talons[0] = &aux; //Place talon objects at arbitrary positions in array
	// talons[1] = &aux1;
	// talons[2] = &i2c;
	
	// bool hasCriticalError = false;
	// bool hasError = false;
	// for(int i = 0; i < numTalons; i++) { //Initialize all Talons //DEBUG!
	// 	talons[i]->begin(Time.now(), hasCriticalError, hasError);
	// }
	// logger.enableI2C_External(false); //Turn off connection to 
	logger.enableI2C_Global(true); //Connect to external bus to talk to sensors/Talons
	logger.enableI2C_OB(false);
	for(int port = 1; port <= Kestrel::numTalonPorts; port++) { //Test all ports
		logger.enableData(port, true); //Turn on specific channel
		logger.enablePower(port, false); 
		logger.enablePower(port, true); 
		// logger.enableAuxPower(true);
		// logger.enableI2C_Global(true);
		// logger.enableI2C_OB(false);
		// delay(1);//DEBUG!
		unsigned long localTime = millis();
		int error = 0;
		while((millis() - localTime) < 10) { //Wait up to 10ms for connection to be established 
			Wire.beginTransmission(0);
			error = Wire.endTransmission();
			if(error == 0) break; //Exit loop once we are able to connect with Talon 
		}
		quickTalonShutdown(); //Quickly disables power to all ports on I2C or SDI talons, this is a kluge 
		for(int t = 0; t < numTalons; t++) { //Iterate over all Talon objects
			if(talonsToTest[t]->getTalonPort() == 0) { //If port not already specified 
				Serial.print("New Talon: ");
				Serial.println(t); 
				// logger.enableAuxPower(false); //Turn aux power off, then configure port to on, then switch aux power back for faster response
				// logger.enablePower(port, true); //Toggle power just before testing to get result within 10ms
				// logger.enablePower(port, false);
				if(talonsToTest[t]->isPresent()) { //Test if that Talon is present, if it is, configure the port
					talonsToTest[t]->setTalonPort(port);
					talons[port - 1] = talonsToTest[t]; //Copy test talon object to index location in talons array
					Serial.print("Talon Port Result "); //DEBUG!
					Serial.print(t);
					Serial.print(": ");
					Serial.println(talonsToTest[t]->getTalonPort());
					break; //Exit the interation after the first one tests positive 
				}
			}
		}
		logger.enableData(port, false); //Turn port back off
	}
	// talons[aux.getTalonPort() - 1] = &aux; //Place talon objects at detected positions in array
	// talons[aux1.getTalonPort() - 1] = &aux1; 
	// talons[i2c.getTalonPort() - 1] = &i2c;
	bool dummy;
	bool dummy1;
	for(int i = 0; i < Kestrel::numTalonPorts - 1; i++) {
		if(talons[i] && talons[i]->getTalonPort() > 0) {
			Serial.print("BEGIN TALON: "); //DEBUG!
			Serial.print(talons[i]->getTalonPort()); 
			Serial.print(",");
			Serial.println(i);
			if(talons[i]->talonInterface == BusType::SDI12) {
				Serial.println("SET FOR SDI12 SEL"); //DEBUG!
				logger.setDirection(talons[i]->getTalonPort(), HIGH); //If the talon is an SDI12 interface type, set port to use serial interface
			}
			else if(talons[i]->talonInterface != BusType::CORE) logger.setDirection(talons[i]->getTalonPort(), LOW); //Otherwise set talon to use GPIO interface, unless bus type is core, in which case ignore it
			logger.enablePower(i + 1, true); //Turn on specific channel
			logger.enableData(i + 1, true);
			if(logger.getFault(talons[i]->getTalonPort())) { //Only toggle power if there is a fault on that Talon line
				logger.enablePower(i + 1, true); //Toggle power just before testing to get result within 10ms
				logger.enablePower(i + 1, false); 
				logger.enablePower(i + 1, true);
			} 
			
			logger.configTalonSense(); //Setup to allow for current testing 
			// Serial.println("TALON SENSE CONFIG DONE"); //DEBUG!
			// Serial.flush(); //DEBUG!
			// logger.enableI2C_Global(true);
			// logger.enableI2C_OB(false);
			// talons[i]->begin(Time.now(), dummy, dummy1); //If Talon object exists and port has been assigned, initialize it //DEBUG!
			talons[i]->begin(logger.getTime(), dummy, dummy1); //If Talon object exists and port has been assigned, initialize it //REPLACE getTime! 
			// talons[i]->begin(0, dummy, dummy1); //If Talon object exists and port has been assigned, initialize it //REPLACE getTime! 
			// Serial.println("TALON BEGIN DONE"); //DEBUG!
			// Serial.flush(); //DEBUG!
			// delay(10000); //DEBUG!
			logger.enableData(i + 1, false); //Turn data back off to prevent conflict 
			// Serial.println("ENABLE DATA DONE"); //DEBUG!
			// Serial.flush(); //DEBUG!
			// delay(10000); //DEBUG!
		}
	}
	return 0; //DEBUG!
}

/**
 * @brief Auto-detects sensors connected to Talon boards
 * 
 * Scans all ports on all detected Talon boards to find connected sensors.
 * This allows the system to automatically adapt to different sensor 
 * configurations, enabling plug-and-play functionality.
 *
 * @param dummyStr Unused parameter (allows function to be called from Particle cloud)
 * @return int 0 on success
 */
int detectSensors(String dummyStr)
{
	/////////////// SENSOR AUTO DETECTION //////////////////////
	for(int t = 0; t < Kestrel::numTalonPorts; t++) { //Iterate over each Talon
	// Serial.println(talons[t]->talonInterface); //DEBUG!
	// Serial.print("DETECT ON TALON: "); //DEBUG!
	// Serial.println(t);
	// Serial.flush();
	// if(talons[t]) {
	// 	delay(5000);
	// 	Serial.println("TALON EXISTS"); //DEBUG!
	// 	Serial.flush();
	// }
	// else {
	// 	delay(5000);
	// 	Serial.println("TALON NOT EXISTS"); //DEBUG!
	// 	Serial.flush();
	// }
	// delay(5000);
	// if(talons[t]->talonInterface != BusType::NONE) {
	// 	delay(5000);
	// 	Serial.println("TALON NOT NONE"); //DEBUG!
	// 	Serial.flush();
	// }
	// else {
	// 	delay(5000);
	// 	Serial.println("TALON NONE"); //DEBUG!
	// 	Serial.flush();
	// }
	// delay(10000); //DEBUG!
		// Serial.println(talons[t]->talonInterface); //DEBUG!
		if(talons[t] && talons[t]->talonInterface != BusType::NONE && talons[t]->getTalonPort() != 0) { //Only proceed if Talon has a bus which can be iterated over, and the talon in question exists and has been detected 
			logger.enableData(talons[t]->getTalonPort(), true); //Turn on specific channel
			// logger.enableI2C_Global(true);
			// logger.enableI2C_OB(false);
			talons[t]->disableDataAll(); //Turn off all data ports on Talon
			for(int p = 1; p <= talons[t]->getNumPorts(); p++) { //Iterate over each port on given Talon
				// talons[t]->enablePower(p, true); //Turn data and power on for specific channel
				talons[t]->enableData(p, true);
				delay(10); //Wait to make sure sensor is responsive after power up command 
				Serial.print("Testing Port: "); //DEBUG!
				Serial.print(t + 1);
				Serial.print(",");
				Serial.println(p);
				for(int s = 0; s < numSensors; s++) { //Iterate over all sensors objects
					if(sensors[s]->getTalonPort() == 0 && talons[t]->talonInterface == sensors[s]->sensorInterface) { //If Talon not already specified AND sensor bus is compatible with Talon bus
						Serial.print("Test Sensor: "); //DEBUG!
						Serial.println(s);
						if(sensors[s]->isPresent()) { //Test if that sensor is present, if it is, configure the port
							sensors[s]->setTalonPort(t + 1);
							sensors[s]->setSensorPort(p);
							if(sensors[s]->keepPowered == true) talons[sensors[s]->getTalonPort() - 1]->keepPowered = true; //If any of the sensors on a Talon require power, set the flag for the Talon
							Serial.print("Sensor Found:\n\t"); //DEBUG!
							Serial.println(sensors[s]->getTalonPort());
							Serial.print('\t');
							Serial.println(sensors[s]->getSensorPort());
							// Serial.print("Talon Port Result "); //DEBUG!
							// Serial.print(t);
							// Serial.print(": ");
							// Serial.println(talons[t]->getTalonPort());
							// talons[t]->enableData(p, false);
							break; //Exit the interation after the first sensor tests positive 
						}
						delay(10); //Wait in between sensor calls
						// talons[t]->enableData(p, false);
					}
				}
				talons[t]->enableData(p, false); //Turn data back off when done
			}
			logger.enableData(talons[t]->getTalonPort(), false); //Turn port back off
		}
		// Serial.print("NEXT TALON"); //DEBUG!
		// Serial.flush();
	}
	return 0; //DEBUG!
}

/**
 * @brief Sets the node identifier for the system
 * 
 * Allows setting a custom node ID for the device through the Particle cloud.
 * This ID is used in all data packets to identify the source of the data.
 *
 * @param nodeID String containing the desired node identifier
 * @return int 0 on success, -1 if the node ID format is invalid
 */
int setNodeID(String nodeID)
{
	// Validate node ID length
	if(nodeID.length() > 8 || nodeID.length() < 0) return -1; //Return failure if string is not valid 
	else {
		globalNodeID = nodeID; //If string passed in is valid, copy it to the global value
		return 0;
	}
}

/**
 * @brief Manually triggers a sensor data collection cycle
 * 
 * This function allows manually triggering a data collection cycle from
 * the Particle cloud. It can either store data in FRAM for later backhaul
 * or immediately send it to the cloud.
 *
 * @param dummy If "true", data is stored in FRAM and backhauled; otherwise direct cloud publish
 * @return int 1 on success
 */
int takeSample(String dummy)
{
	logger.wake(); //Wake logger in case it was sleeping
	wakeSensors(); //Wake up sensors from sleep
	
	if(dummy == "true") { //If told to use backhaul, use normal FRAM method
		// Store in FRAM and backhaul
		fileSys.writeToFRAM(getDataString(), DataType::Data, DestCodes::Both); 
		fileSys.dumpFRAM(); //Dump data
	}
	else {
		// Fast direct publish to Particle cloud
		fileSys.writeToParticle(getDataString(), "data/v2"); //Otherwise fast return
	}
	
	sleepSensors(); // Put sensors back to sleep
	logger.sleep();  // Put logger back to sleep
	return 1;
}

/**
 * @brief Executes system commands based on numeric codes
 * 
 * Allows remote execution of various system commands through the Particle cloud
 * using numeric codes. Commands include releasing the watchdog timer,
 * collecting diagnostics, collecting data, erasing FRAM, etc.
 *
 * @param command String containing the numeric command code
 * @return int 1 on success, -1 if command not recognized
 */
int commandExe(String command)
{
	// Command 300: Release the watchdog timer (allows system to reset)
	if(command == "300") {
		logger.releaseWDT();
		return 1; //DEBUG!
	}
	if(command == "102") {
		logger.wake(); //Wake logger in case it was sleeping
		wakeSensors(); //Wake up sensors from sleep
		fileSys.writeToParticle(getDiagnosticString(2), "diagnostic/v2"); 
		sleepSensors(); //
		logger.sleep();
		return 1; //DEBUG!
	}
	if(command == "103") {
		logger.wake(); //Wake logger in case it was sleeping
		wakeSensors(); //Wake up sensors from sleep
		fileSys.writeToParticle(getDiagnosticString(3), "diagnostic/v2"); 
		sleepSensors(); //
		logger.sleep();
		return 1; //DEBUG!
	}
	if(command == "104") {
		logger.wake(); //Wake logger in case it was sleeping
		wakeSensors(); //Wake up sensors from sleep
		fileSys.writeToParticle(getDiagnosticString(4), "diagnostic/v2"); 
		sleepSensors(); //
		logger.sleep();
		return 1; //DEBUG!
	}
	if(command == "111") {
		logger.wake(); //Wake logger in case it was sleeping
		wakeSensors(); //Wake up sensors from sleep
		fileSys.writeToParticle(getDataString(), "data/v2"); 
		sleepSensors(); //
		logger.sleep();
		return 1; //DEBUG!
	}
	if(command == "120") {
		fileSys.writeToParticle(getErrorString(), "error/v2"); 
		return 1; //DEBUG!
	}
	if(command == "130") {
		logger.wake(); //Wake logger in case it was sleeping
		wakeSensors(); //Wake up sensors from sleep
		fileSys.writeToParticle(getMetadataString(), "metadata/v2"); 
		sleepSensors(); //
		logger.sleep();
		return 1; //DEBUG!
	}
	if(command == "401") {
		fileSys.wake();
		fileSys.dumpFRAM();
		fileSys.sleep();
		return 1;
	}
	if(command == "410") {
		fileSys.wake();
		fileSys.eraseFRAM(); //Clear FRAM and start over
		fileSys.sleep();
		return 1; //DEBUG!
	}
	else {
		return -1; //Return unknown command 
	}
}

/**
 * @brief Restarts the system
 * 
 * Provides a way to remotely restart the system through the Particle cloud.
 * Can perform either a hard reset (immediate) or a soft reset (waits for cloud connection).
 *
 * @param resetType String "hard" for immediate reset, anything else for soft reset
 * @return int 1 on success (though function typically doesn't return due to reset)
 */
int systemRestart(String resetType)
{
	if(resetType.equalsIgnoreCase("hard")) {
		// Hard reset - immediate restart without waiting for cloud
		System.reset(RESET_NO_WAIT); //Perform a hard reset
	}
	else {
		// Soft reset - attempts to notify the cloud before resetting
		System.reset(); //Attempt to inform cloud of a reset first 
	}
	return 1;
}

/**
 * @brief Configures system-wide power saving mode
 * 
 * Sets the power saving mode for the entire system, including all
 * sensors and Talon boards. This affects how aggressively the system
 * conserves power between measurements.
 *
 * @param desiredPowerSaveMode Integer value from PowerSaveModes enum 
 *                            (PERFORMANCE, BALANCED, LOW_POWER, ULTRA_LOW_POWER)
 * @return int 0 on success
 */
int configurePowerSave(int desiredPowerSaveMode)
{
	// Update the global power save mode setting
	powerSaveMode = desiredPowerSaveMode; //Configure global flag
	
	// Configure power save mode for all sensors
	for(int s = 0; s < numSensors; s++) { //Iterate over all sensors objects
		sensors[s]->powerSaveMode = desiredPowerSaveMode; //Set power save mode for all sensors
	}

	// Configure power save mode for all Talons
	for(int t = 0; t < numTalons; t++)  { //Iterate over all talon objects
		talonsToTest[t]->powerSaveMode = desiredPowerSaveMode; //Set power save mode for all talons
	}
	return 0; //DEBUG!
}