/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/schul/Documents/Firmware_-_FlightControl-Demo/src/FlightControl_Demo.ino"
/*
 * Project FlightControl_Demo
 * Description:
 * Author:
 * Date:
 */

// #define RAPID_START  //Does not wait for remote connection on startup
void setup();
void loop();
void logEvents(uint8_t type);
String getErrorString();
String getDataString();
String getDiagnosticString(uint8_t level);
String getMetadataString();
String initSensors();
void quickTalonShutdown();
bool serialConnected();
void systemConfig();
int detectTalons(String dummyStr);
int detectSensors(String dummyStr);
int setNodeID(String nodeID);
#line 9 "c:/Users/schul/Documents/Firmware_-_FlightControl-Demo/src/FlightControl_Demo.ino"
#define USE_CELL  //System attempts to connect to cell
#include <AuxTalon.h>
#include <PCAL9535A.h>
#include <Sensor.h>
#include <Talon.h>
#include <Kestrel.h>
#include <KestrelFileHandler.h>
#include <Haar.h>
#include <SO421.h>
#include <SP421.h>
#include <TEROS11.h>
#include <I2CTalon.h>
#include <SDI12Talon.h>
#include <Gonk.h>
#include <vector>
#include <memory>

const String firmwareVersion = "1.1.0";

const int backhaulCount = 1; //Number of log events before backhaul is performed 
const unsigned long maxConnectTime = 180000; //Wait up to 180 seconds for systems to connect 
const unsigned long indicatorTimeout = 300000; //Wait for up to 5 minutes with indicator lights on
const unsigned long logPeriod = 300; //Wait 60 seconds between logs

Kestrel logger;
KestrelFileHandler fileSys(logger);
AuxTalon aux(0, 0x14); //Instantiate AUX talon with deaults - null port and hardware v1.4
AuxTalon aux1(0, 0x14); //Instantiate AUX talon with alt - null port and hardware v1.4
I2CTalon i2c(0, 0x21); //Instantiate I2C talon with alt - null port and hardware v2.1
SDI12Talon sdi12(0, 0x14); //Instantiate SDI12 talon with alt - null port and hardware v1.4
PCAL9535A ioAlpha(0x20);
PCAL9535A ioBeta(0x21);
Haar haar(0, 0, 0x20); //Instantiate Haar sensor with default ports and version v2.0
Haar haar1(0, 0, 0x20); //Instantiate Haar sensor with default ports and version v2.0
SO421 apogeeO2(sdi12, 0, 0); //Instantiate O2 sensor with default ports and unknown version, pass over SDI12 Talon interface
SP421 apogeeSolar(sdi12, 0, 0); //Instantiate solar sensor with default ports and unknown version, pass over SDI12 Talon interface
TEROS11 soil(sdi12, 0, 0); //Instantiate soil sensor with default ports and unknown version, pass over SDI12 Talon interface
Gonk battery(5); //Instantiate with defaults, manually set to port 5 

const uint8_t numSensors = 8; 
const uint8_t numTalons = 3;
String globalNodeID = ""; //Store current node ID

Talon* talons[Kestrel::numTalonPorts]; //Create an array of the total possible length
Sensor* const sensors[numSensors] = {
	&aux,
	// &aux1,
	&i2c,
	&haar,
	// &haar1, 
	&logger,
	&sdi12,
	&battery,
	// &apogeeO2,
	&soil,
	&apogeeSolar
};

Talon* talonsToTest[numTalons] = {
	&aux,
	// &aux1,
	&i2c,
	&sdi12
};

// namespace Pins { //Use for B402
// 	constexpr uint16_t WD_HOLD  = D2;
// 	constexpr uint16_t SD_CS    = D8;
// 	constexpr uint16_t Clock_INT 	= D22;
// 	constexpr uint16_t TALON1_GPIOA = A3;
// 	constexpr uint16_t TALON1_GPIOB = D7;
// 	constexpr uint16_t TALON2_GPIOA = A2;
// 	constexpr uint16_t TALON2_GPIOB = D6;
// 	constexpr uint16_t TALON3_GPIOA = A1;
// 	constexpr uint16_t TALON3_GPIOB = D5;
// 	constexpr uint16_t I2C_GLOBAL_EN = D23; //FIX!
// 	constexpr uint16_t I2C_OB_EN = A6; //FIX!
// }

namespace PinsIO { //For Kestrel v1.1
	constexpr uint16_t VUSB = 5;
}

namespace PinsIOAlpha {
	constexpr uint16_t I2C_EXT_EN = 10;
	constexpr uint16_t SD_CD = 8;
	constexpr uint16_t SD_EN = 12;
	constexpr uint16_t AUX_EN = 15;
	constexpr uint16_t CE = 11;
	constexpr uint16_t LED_EN = 13;
}

namespace PinsIOBeta { //For Kestrel v1.1
	constexpr uint16_t SEL1 = 0;
	constexpr uint16_t SEL2 = 4;
	constexpr uint16_t SEL3 = 8;
	constexpr uint16_t SEL4 = 12;
	constexpr uint16_t I2C_EN1 = 1;
	constexpr uint16_t I2C_EN2 = 5;
	constexpr uint16_t I2C_EN3 = 9;
	constexpr uint16_t I2C_EN4 = 13;
	constexpr uint16_t EN1 = 3;
	constexpr uint16_t EN2 = 7;
	constexpr uint16_t EN3 = 11;
	constexpr uint16_t EN4 = 15;
	constexpr uint16_t FAULT1 = 2;
	constexpr uint16_t FAULT2 = 6;
	constexpr uint16_t FAULT3 = 10;
	constexpr uint16_t FAULT4 = 14;
}

// SYSTEM_MODE(MANUAL);
SYSTEM_MODE(SEMI_AUTOMATIC);
// SYSTEM_THREAD(ENABLED); //USE FOR FASTER STARTUP
SYSTEM_THREAD(DISABLED); 
// SYSTEM_MODE(AUTOMATIC);
int detectTalons(String dummyStr = "");
int detectSensors(String dummyStr = "");

void setup() {
	// System.disableReset(); //DEBUG!
	// RESET_REASON_PIN_RESET
	// RESET_REASON_POWER_DOWN
	System.enableFeature(FEATURE_RESET_INFO); //DEBUG!
	if(System.resetReason() != RESET_REASON_POWER_DOWN) {
		//DEBUG! Set safe mode 
		Particle.connect(); //DEBUG! //If reset not caused by power switch, assume something bad happened, just connect to particle straight away
	}
	//////////// MANUAL POSITIONING //////////////////
  	// talons[aux.getTalonPort()] = &aux; //Place talon objects at coresponding positions in array
	// talons[aux1.getTalonPort()] = &aux1;
	time_t startTime = millis();
	Particle.function("nodeID", setNodeID);
	Particle.function("findSensors", detectSensors);
	Particle.function("findTalons", detectTalons);
	Serial.begin(1000000); 
	waitFor(serialConnected, 10000); //DEBUG! Wait until serial starts sending or 10 seconds 
	// Serial.println(System.resetReason()); //DEBUG!
	bool hasCriticalError = false;
	bool hasError = false;
	// logger.begin(Time.now(), hasCriticalError, hasError); //Needs to be called the first time with Particle time since I2C not yet initialized 
	logger.begin(0, hasCriticalError, hasError); //Called with 0 since time collection system has not been initialized 
	logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::INIT);
	bool batState = logger.testForBat(); //Check if a battery is connected
	logger.enableI2C_OB(false);
	logger.enableI2C_External(true); //Connect to Gonk I2C port
	logger.enableI2C_Global(true);
	if(batState) battery.setIndicatorState(GonkIndicatorMode::SOLID); //Turn on charge indication LEDs during setup 
	else battery.setIndicatorState(GonkIndicatorMode::BLINKING); //If battery not switched on, set to blinking 
	fileSys.begin(false); //Initialzie, but do not attempt backhaul

	//   I2C_OnBoardEn(true); 	
	// Wire.setClock(400000); //Confirm operation in fast mode
	// Wire.begin();
	logger.enableI2C_Global(false); //Connect to internal bus
	logger.enableI2C_OB(true);
	ioAlpha.begin(); //RESTORE
	ioBeta.begin(); //RESTORE
	// ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT); //DEBUG
	// ioBeta.digitalWrite(PinsIOBeta::SEL2, LOW); //DEBUG
	ioAlpha.pinMode(PinsIOAlpha::LED_EN, OUTPUT);
	ioAlpha.digitalWrite(PinsIOAlpha::LED_EN, LOW); //Turn on LED indicators 
	logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::IDLE);
	waitFor(serialConnected, 10000); //DEBUG! Wait until serial starts sending or 10 seconds
	if(Serial.available()) {
		//COMMAND MODE!
		logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::COMMAND);
		systemConfig();
	}
	logger.setIndicatorState(IndicatorLight::ALL,IndicatorMode::WAITING);
	Particle.connect(); //Once passed attempted serial connect, try to connect to particle 

	////////// ADD INTERRUPTS!
	// for(int i = 1; i <= Kestrel::numTalonPorts; i++) { //Iterate over ALL ports
	// 	logger.enablePower(i, true); //Turn on all power by default
	// 	logger.enablePower(i, false); //Toggle power to reset
	// 	logger.enablePower(i, true); 
	// 	logger.enableData(i, false); //Turn off all data by default
	// }

	detectTalons();
	detectSensors();

	// I2C_OnBoardEn(false);	
	// I2C_GlobalEn(true);

	// bool hasCriticalError = false;
	// bool hasError = false;
	// logger.enableData(4, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	// String initDiagnostic = aux.begin(Time.now(), hasCriticalError, hasError);

	String initDiagnostic = initSensors();
	Serial.print("DIAGNOSTIC: ");
	Serial.println(initDiagnostic);
	// fileSys.writeToParticle(initDiagnostic, "diagnostic"); 
	// // logger.enableSD(true);
	// fileSys.writeToSD(initDiagnostic, "Dummy.txt");

	#ifndef RAPID_START  //Only do this if not rapid starting
	while((!Particle.connected() || logger.gps.getFixType() == 0) && (millis() - startTime) < maxConnectTime) { //Wait while at least one of the remote systems is not connected 
		if(Particle.connected()) logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::PASS); //If cell is connected, set to PASS state
		if(logger.gps.getTimeValid() == true) {
			if(logger.gps.getFixType() >= 2) { //If you get a 2D fix or better, pass GPS 
				logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PASS); 
			}
			else logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PREPASS); //If time is good, set preliminary pass only
		}
		delay(5000); //Wait 5 seconds between each check to not lock up the process //DEBUG!
	}
	#endif
	
	if(Particle.connected()) logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::PASS); //Catches connection of cell is second device to connect
	else logger.setIndicatorState(IndicatorLight::CELL, IndicatorMode::ERROR); //If cell still not connected, display error
	if(logger.gps.getFixType() >= 2) logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::PASS); //Catches connection of GPS is second device to connect
	else logger.setIndicatorState(IndicatorLight::GPS, IndicatorMode::ERROR); //If GPS fails to connect after period, set back to error
	fileSys.tryBackhaul(); //See if we can backhaul any unsent logs

	logEvents(3); //Grab data log with metadata
	fileSys.dumpFRAM(); //Backhaul this data right away
	// Particle.publish("diagnostic", initDiagnostic);

	// logger.enableData(3, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	// aux1.begin(Time.now(), hasCriticalError, hasError);
  	//FIX! RESPOND TO ERROR RESULTS! 
}

void loop() {
  // aux.sleep(false);
  static int count = 1; //Keep track of number of cycles

	if(System.millis() > indicatorTimeout) {
		logger.setIndicatorState(IndicatorLight::ALL, IndicatorMode::NONE); //Turn LED indicators off if it has been longer than timeout since startup (use system.millis() which does not rollover)
		logger.enableI2C_OB(false);
		logger.enableI2C_External(true); //Connect to Gonk I2C port
		logger.enableI2C_Global(true);
		battery.setIndicatorState(GonkIndicatorMode::PUSH_BUTTON); //Turn off indicator lights on battery, return to push button control
		logger.enableI2C_External(false); //Turn off external I2C
	}
	bool alarm = logger.waitUntilTimerDone(); //Wait until the timer period has finished 
	if(alarm) Serial.println("RTC Wakeup"); //DEBUG!
	else Serial.println("Timeout Wakeup"); //DEBUG!
	if((count % 1) == 0) logEvents(1);
	if((count % 10) == 0) logEvents(2);
	if((count % 5) == 0) logEvents(3);
	Serial.println("Log Done"); //DEBUG!
	logger.feedWDT(); 
	logger.startTimer(logPeriod); //Start timer as soon done reading sensors

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

	if((count % backhaulCount) == 0) {
		Serial.println("BACKHAUL"); //DEBUG!
		fileSys.dumpFRAM(); //dump FRAM every Nth log
	}
	count++;

}

void logEvents(uint8_t type)
{
	String diagnostic = "";
	String errors = "";
	String metadata = "";
	String data = "";
	Serial.print("LOG: "); //DEBUG!
	Serial.println(type); 
	
	switch(type) {
		case 1: //Standard, short interval, log
			
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
				fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
			}
			fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
			fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
		break;

		case 2: //Low period log with diagnostics
			data = getDataString();
			diagnostic = getDiagnosticString(3);
			errors = getErrorString();
			// logger.enableI2C_OB(true);
			// logger.enableI2C_Global(false);
			if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
			fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
			fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
		break;

		case 3: //Daily log event with increased diagnostics and metadata
			data = getDataString();
			// diagnostic = getDiagnosticString(2);
			metadata = getMetadataString();
			errors = getErrorString();
			// logger.enableI2C_OB(true);
			// logger.enableI2C_Global(false);
			if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
			fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
			// fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::Both);
			fileSys.writeToFRAM(metadata, DataType::Metadata, DestCodes::Both);
		break;

	}
}
String getErrorString()
{
	unsigned long numErrors = 0; //Used to keep track of total errors across all devices 
	String errors = "{\"Error\":{";
	errors = errors + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	errors = errors + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosTimeString() + "],";
	if(globalNodeID != "") errors = errors + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else errors = errors + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	errors = errors + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	errors = errors + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	errors = errors + "\"Sensors\":[";
	for(int i = 0; i < numSensors; i++) {
		if(sensors[i]->totalErrors() > 0) {
			numErrors = numErrors + sensors[i]->totalErrors(); //Increment the total error count
			if(!errors.endsWith("[")) errors = errors + ","; //Only append if not first entry
			errors = errors + sensors[i]->getErrors();
		}
	}
	errors = errors + "]}}"; //Close data
	Serial.print("Num Errors: "); //DEBUG!
	Serial.println(numErrors); 
	if(numErrors > 0) return errors;
	else return ""; //Return null string if no errors reported 
}

String getDataString()
{
	String data = "{\"Data\":{";
	data = data + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	data = data + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosTimeString() + "],";
	if(globalNodeID != "") data = data + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else data = data + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	data = data + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	data = data + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	data = data + "\"Sensors\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enablePower(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon, only if not core system
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon
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
			talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn on data for the given port on the Talon
			// bool dummy1;
			// bool dummy2;
			// sensors[i]->begin(Time.now(), dummy1, dummy2); //DEBUG!
		}
		// delay(100); //DEBUG!
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		data = data + sensors[i]->getData(logger.getTime()); //DEBUG! REPLACE!
		if(i + 1 < numSensors) data = data + ","; //Only append if not last entry
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) {
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), false); //Turn off power for the given port on the Talon //DEBUG!
		}
	}
	data = data + "]}}"; //Close data
	return data;
}

String getDiagnosticString(uint8_t level)
{
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosTimeString() + "],";
	if(globalNodeID != "") leader = leader + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else leader = leader + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Level\":" + String(level) + ",\"Sensors\":["; //Concatonate number of sensors and level 
	String closer = "]}}";
	String output = leader;

	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enablePower(sensors[i]->getTalonPort(), true);
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
			
		}

  		String diagnostic = sensors[i]->selfDiagnostic(level, logger.getTime());
		if(output.length() - output.lastIndexOf('\n') + diagnostic.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
			if(i > 0) output = output + ","; //Add preceeding comma if not the first entry
			output = output + diagnostic; //Append result 
			// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
		}
		else {
			output = output + closer + "\n"; //End this packet
			output = output + leader + diagnostic; //Start a new packet and add new payload 
		}
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) {
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
			// talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), false); //Turn off power for the given port on the Talon //DEBUG!
		}
		
	}
	output = output + "]}}"; //Close diagnostic
	return output;
}

String getMetadataString()
{
	String metadata = "{\"Metadata\":{";
	metadata = metadata + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	metadata = metadata + "\"Loc\":[" + logger.getPosLat() + "," + logger.getPosLong() + "," + logger.getPosTimeString() + "],";
	if(globalNodeID != "") metadata = metadata + "\"Node ID\":\"" + globalNodeID + "\","; //Concatonate node ID
	else metadata = metadata + "\"Device ID\":\"" + System.deviceID() + "\","; //If node ID not initialized, use device ID
	metadata = metadata + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	metadata = metadata + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	metadata = metadata + "\"System\":{";
	metadata = metadata + "\"Firm\":\"" + firmwareVersion + "\",";
	metadata = metadata + "\"OS\":\"" + System.version() + "\",";
	metadata = metadata + "\"ID\":\"" + System.deviceID() + "\"},";
	//FIX! Add support for device name 
	metadata = metadata + "\"Sensors\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
			// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getSensorPort() > 0 && sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
		}
		// logger.enablePower(sensors[i]->getTalon(), true); //Turn on power to port
		// logger.enableData(sensors[i]->getTalon(), true); //Turn on data to port
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		metadata = metadata + sensors[i]->getMetadata();
		if(i + 1 < numSensors) metadata = metadata + ","; //Only append if not last entry
	}

	metadata = metadata + "]}}"; //Close metadata
	return metadata;
}

String initSensors()
{
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + logger.getTimeString() + ","; //Concatonate time
	leader = leader + "\"Packet ID\":" + logger.getMessageID() + ","; //Concatonate unique packet hash
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Sensors\":["; //Concatonate number of sensors 
	
	String closer = "]}}";
	String output = leader;
	bool reportCriticalError = false; //Used to keep track of the global status of the error indications for all sensors
	bool reportError = false;
	bool missingSensor = false;
	// output = output + "\"Devices\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		if(sensors[i]->sensorInterface != BusType::CORE) logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
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
			talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
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

  		String initDiagnostic = sensors[i]->begin(logger.getTime(), hasCriticalError, hasError);
		if(hasError) reportError = true; //Set if any of them throw an error
		if(hasCriticalError) reportCriticalError = true; //Set if any of them throw a critical error
		if(output.length() - output.lastIndexOf('\n') + initDiagnostic.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
			if(i > 0) output = output + ","; //Add preceeding comma if not the first entry
			output = output + initDiagnostic; //Append result 
			// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
		}
		else {
			output = output + closer + "\n"; //End this packet
			output = output + leader + initDiagnostic; //Start a new packet and add new payload 
		}
		
	}
	if(missingSensor) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR);
	else logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS); //If no errors are reported, set to pass state
	//FIX! Replace!
	// if(reportCriticalError) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR_CRITICAL);
	// else if(reportError) logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::ERROR); //Only set minimal error state if critical error is not thrown
	// else logger.setIndicatorState(IndicatorLight::SENSORS, IndicatorMode::PASS); //If no errors are reported, set to pass state
	
	output = output + "]}}"; //Close diagnostic
	return output;
}

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

	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x06); //Point to config port
	Wire.write(0xF0); //Set pins 1 - 4 as output
	Wire.endTransmission();

	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x02); //Point to output port
	Wire.write(0xF0); //Set pints 1 - 4 low
	Wire.endTransmission();

	Wire.beginTransmission(0x25); //Talk to SDI12 Talon
	Wire.write(0x00); //Point to port reg
	// Wire.write(0xF0); //Set pints 1 - 4 low
	Wire.endTransmission();

	Wire.requestFrom(0x25, 1); 
	Wire.read(); //Read back current value
}

bool serialConnected() //Used to check if a monitor has been connected at the begining of operation for override control 
{
	if(Serial.available() > 0) return true;
	else return false;
}

void systemConfig()
{
	Serial.println("HALT: Entered Command Mode - Here be Dragons"); //DEBUG!
	static int ReadLength = 0;
  	String ReadString;
	char ReadArray[25] = {0};
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

				if(ReadString.equalsIgnoreCase("Exit")) {
					return; //Exit the setup function
				}
			}
		}
	}
}

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
	logger.enableI2C_Global(true); //Connect to external bus to talk to sensors/Talons
	logger.enableI2C_OB(false);
	for(int port = 1; port <= Kestrel::numTalonPorts; port++) { //Test all ports
		logger.enableData(port, true); //Turn on specific channel
		for(int t = 0; t < numTalons; t++) { //Iterate over all Talon objects
			if(talonsToTest[t]->getTalonPort() == 0) { //If port not already specified 
				Serial.print("New Talon: ");
				Serial.println(t); 
				// logger.enableAuxPower(false); //Turn aux power off, then configure port to on, then switch aux power back for faster response
				// logger.enablePower(port, true); //Toggle power just before testing to get result within 10ms
				// logger.enablePower(port, false);
				logger.enablePower(port, false); 
				logger.enablePower(port, true); 
				// logger.enableAuxPower(true);
				// logger.enableI2C_Global(true);
				// logger.enableI2C_OB(false);
				quickTalonShutdown(); //Quickly disables power to all ports on I2C or SDI talons, this is a kluge 
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
	for(int i = 0; i < Kestrel::numTalonPorts; i++) {
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
			// logger.enableI2C_Global(true);
			// logger.enableI2C_OB(false);
			// talons[i]->begin(Time.now(), dummy, dummy1); //If Talon object exists and port has been assigned, initialize it //DEBUG!
			talons[i]->begin(logger.getTime(), dummy, dummy1); //If Talon object exists and port has been assigned, initialize it //REPLACE getTime! 
			logger.enableData(i + 1, false); //Turn data back off to prevent conflict 
		}
	}
	return 0; //DEBUG!
}

int detectSensors(String dummyStr)
{
	/////////////// SENSOR AUTO DETECTION //////////////////////
	for(int t = 0; t < numTalons; t++) { //Iterate over each Talon
		// Serial.println(talons[t]->talonInterface); //DEBUG!
		if(talons[t]->talonInterface != BusType::NONE && talons[t]) { //Only proceed if Talon has a bus which can be iterated over, and the talon in question exists
			logger.enableData(talons[t]->getTalonPort(), true); //Turn on specific channel
			// logger.enableI2C_Global(true);
			// logger.enableI2C_OB(false);
			talons[t]->disableDataAll(); //Turn off all data ports on Talon
			for(int p = 1; p <= talons[t]->getNumPorts(); p++) { //Iterate over each port on given Talon
				talons[t]->enablePower(p, true); //Turn data and power on for specific channel
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
	}
	return 0; //DEBUG!
}

int setNodeID(String nodeID)
{
	if(nodeID.length() > 8 || nodeID.length() < 0) return -1; //Return failure if string is not valid 
	else {
		globalNodeID = nodeID; //If string passed in is valid, copy it to the global value
		return 0;
	}
}