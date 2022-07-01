/*
 * Project FlightControl_Demo
 * Description:
 * Author:
 * Date:
 */

#include <AuxTalon.h>
#include <PCAL9535A.h>
#include <Sensor.h>
#include <Talon.h>
#include <Kestrel.h>
#include <KestrelFileHandler.h>
#include <Haar.h>
#include <I2CTalon.h>
#include <vector>
#include <memory>

const String firmwareVersion = "1.0.1";

const int backhaulCount = 3; //Number of log events before backhaul is performed 

Kestrel logger;
KestrelFileHandler fileSys(logger);
AuxTalon aux(0, 0x14); //Instantiate AUX talon with deaults - port 4 and hardware v1.4
AuxTalon aux1(0, 0x14); //Instantiate AUX talon with alt - port 3 and hardware v1.4
I2CTalon i2c(0, 0x21); //Instantiate I2C talon with alt - null port and hardware v2.1
PCAL9535A ioAlpha(0x20);
PCAL9535A ioBeta(0x21);
Haar haar(0, 0, 0x20); //Instantiate Haar sensor with default ports and version v2.0
Haar haar1(0, 0, 0x20); //Instantiate Haar sensor with default ports and version v2.0

const uint8_t numSensors = 6; 
const uint8_t numTalons = 3;
// std::vector<std::unique_ptr<Sensor>> sensors;
Sensor* const sensors[numSensors] = {
	&aux,
	&aux1,
	&i2c,
	&haar,
	&haar1, 
	&logger
};

Talon* talons[Kestrel::numTalonPorts]; //Create an array of the total possible length

// sensors.push_back(aux);
// sensors.push_back(aux1);
// sensors.emlace_back(aux);
// sensors.emlace_back(aux1);

// Sensor sensors[2] = {aux, aux1};

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
SYSTEM_MODE(AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() {
	//////////// MANUAL POSITIONING //////////////////
  	// talons[aux.getTalonPort()] = &aux; //Place talon objects at coresponding positions in array
	// talons[aux1.getTalonPort()] = &aux1;
	
  Serial.begin(115200); 
  waitFor(Serial.isConnected, 30000);
	bool hasCriticalError = false;
	bool hasError = false;
	logger.begin(Time.now(), hasCriticalError, hasError);
//   logger.enableI2C_OB(true);
	// logger.enableSD(true);
	//FIX! Turn on correct things before running file begin
  fileSys.begin();

//   I2C_OnBoardEn(true); 
	Wire.setClock(400000); //Confirm operation in fast mode
	Wire.begin();
	logger.enableI2C_Global(false);
	logger.enableI2C_OB(true);
	ioAlpha.begin(); //RESTORE
	ioBeta.begin(); //RESTORE
	ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT); //DEBUG
	ioBeta.digitalWrite(PinsIOBeta::SEL2, LOW); //DEBUG

//   I2C_OnBoardEn(true); //Turn on internal I2C
//   ioBeta.pinMode(PinsIOBeta::EN4, OUTPUT);
//   ioBeta.pinMode(PinsIOBeta::I2C_EN4, OUTPUT);
//   ioBeta.pinMode(PinsIOBeta::SEL4, OUTPUT);

//   ioBeta.digitalWrite(PinsIOBeta::EN4, HIGH); //Turn on power to port 4
//   ioBeta.digitalWrite(PinsIOBeta::I2C_EN4, HIGH); //Turn on I2C to port 4
//   ioBeta.digitalWrite(PinsIOBeta::SEL4, LOW); //Turn on GPIO connection to port 4
  ////////// ADD INTERRUPTS!
  	// logger.enablePower(3, true);
	for(int i = 1; i <= Kestrel::numTalonPorts; i++) { //Iterate over ALL ports
		logger.enablePower(i, true); //Turn on all power by default
		logger.enableData(i, false); //Turn off all data by default
	}
	
	// logger.enableData(3, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	////////////// AUTO TALON DETECTION ///////////////////////
	talons[0] = &aux; //Place talon objects at arbitrary positions in array
	talons[1] = &aux1;
	talons[2] = &i2c;
	
	// bool hasCriticalError = false;
	// bool hasError = false;
	// for(int i = 0; i < numTalons; i++) { //Initialize all Talons //DEBUG!
	// 	talons[i]->begin(Time.now(), hasCriticalError, hasError);
	// }
	for(int port = 1; port <= Kestrel::numTalonPorts; port++) { //Test all ports
		logger.enableData(port, true); //Turn on specific channel
		for(int t = 0; t < numTalons; t++) { //Iterate over all Talon objects
			if(talons[t]->getTalonPort() == 0) { //If port not already specified 
				Serial.print("New Talon: ");
				Serial.println(t); 
				logger.enableI2C_Global(true);
				logger.enableI2C_OB(false);
				if(talons[t]->isPresent()) { //Test if that Talon is present, if it is, configure the port
					talons[t]->setTalonPort(port);
					Serial.print("Talon Port Result "); //DEBUG!
					Serial.print(t);
					Serial.print(": ");
					Serial.println(talons[t]->getTalonPort());
					break; //Exit the interation after the first one tests positive 
				}
			}
			
		}
		logger.enableData(port, false); //Turn port back off
	}
	talons[aux.getTalonPort() - 1] = &aux; //Place talon objects at detected positions in array
	talons[aux1.getTalonPort() - 1] = &aux1; 
	talons[i2c.getTalonPort() - 1] = &i2c;

	//Assign sensor positions
	for(int t = 0; t < numTalons; t++) { //Iterate over each Talon
		// Serial.println(talons[t]->talonInterface); //DEBUG!
		if(talons[t]->talonInterface != 0) { //Only proceed if Talon has a bus which can be iterated over
			logger.enableData(talons[t]->getTalonPort(), true); //Turn on specific channel
			logger.enableI2C_Global(true);
			logger.enableI2C_OB(false);
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
							talons[t]->enableData(p, false);
							break; //Exit the interation after the first sensor tests positive 
						}
						talons[t]->enableData(p, false);
					}
				}
			}
			logger.enableData(talons[t]->getTalonPort(), false); //Turn port back off
		}
	}

//   I2C_OnBoardEn(false);	
//   I2C_GlobalEn(true);

// 	bool hasCriticalError = false;
// 	bool hasError = false;
// 	logger.enableData(4, true);
// 	logger.enableI2C_OB(false);
// 	logger.enableI2C_Global(true);
//   String initDiagnostic = aux.begin(Time.now(), hasCriticalError, hasError);

	String initDiagnostic = initSensors();
	Serial.print("DIAGNOSTIC: ");
	Serial.println(initDiagnostic);
	fileSys.writeToParticle(initDiagnostic, "diagnostic"); 
	// logger.enableSD(true);
	fileSys.writeToSD(initDiagnostic, "Dummy.txt");
	// Particle.publish("diagnostic", initDiagnostic);

	// logger.enableData(3, true);
	// logger.enableI2C_OB(false);
	// logger.enableI2C_Global(true);
	// aux1.begin(Time.now(), hasCriticalError, hasError);
  //FIX! RESPOND TO ERROR RESULTS! 
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  
  // aux.sleep(false);
  static int count = 1; //Keep track of number of cycles
//   String diagnostic = getDiagnosticString(4);
//   aux.restart();
  
//   String errors = aux.getErrors();
//   String metadata = getMetadataString();
// 	String data = getDataString();
	bool alarm = logger.waitUntilTimerDone(); //Wait until the timer period has finished 
	if(alarm) Serial.println("RTC Wakeup"); //DEBUG!
	else Serial.println("Timeout Wakeup"); //DEBUG!
	if((count % 1) == 0) logEvents(1);
	if((count % 10) == 0) logEvents(2);
	if((count % 5) == 0) logEvents(3);
	logger.startTimer(30); //Start timer as soon done reading sensors

//   Particle.publish("diagnostic", diagnostic);
//   Particle.publish("error", errors);
//   Particle.publish("data", data);
//   Particle.publish("metadata", metadata);

//   Serial.print("DIAGNOSTIC: ");
//   Serial.println(diagnostic);
//   Serial.print("ERROR: ");
//   Serial.println(errors);
//   Serial.print("DATA: ");
//   Serial.println(data);
//   Serial.print("METADATA: ");
//   Serial.println(metadata);

	// logger.enableI2C_OB(true);
	// logger.enableI2C_Global(false);
//   fileSys.writeToFRAM(diagnostic, "diagnostic", DestCodes::Particle);

  if((count % backhaulCount) == 0) {
	Serial.println("BACKHAUL");
	// logger.enableI2C_OB(true);
	// logger.enableI2C_Global(false);
	fileSys.dumpFRAM(); //dump FRAM every 5th log
	// count = 0;
  }
  // aux.sleep(true);
//   delay(10000); //Wait a while //DEBUG!
	count++;

}

// void I2C_GlobalEn(bool State)
// {
// 	pinMode(Pins::I2C_GLOBAL_EN, OUTPUT);
// 	digitalWrite(Pins::I2C_GLOBAL_EN, State);
// }

// void I2C_OnBoardEn(bool State)
// {
// 	pinMode(Pins::I2C_OB_EN, OUTPUT);
// 	digitalWrite(Pins::I2C_OB_EN, State);
// }

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
			diagnostic = getDiagnosticString(4);
			errors = getErrorString(); //Get errors last to wait for error codes to be updated 
			// logger.enableI2C_OB(true);
			// logger.enableI2C_Global(false);
			Serial.println(errors); //DEBUG!
			Serial.println(data); //DEBUG!
			Serial.println(diagnostic); //DEBUG!
			if(errors.equals("") == false) fileSys.writeToFRAM(errors, DataType::Error, DestCodes::Both); //Write value out only if errors are reported 
			fileSys.writeToFRAM(data, DataType::Data, DestCodes::Both);
			fileSys.writeToFRAM(diagnostic, DataType::Diagnostic, DestCodes::SD);
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
	errors = errors + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	errors = errors + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	//   for(int i = 0; i<2; i++) {
		// data = data + "\"Devices\":[" + sensors[0]->getData(Time.now()) + "," + sensors[1]->getData(Time.now()) + "]}}";
		errors = errors + "\"Devices\":[";
		for(int i = 0; i < numSensors; i++) {
			// logger.enableData(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon
			// logger.enableI2C_OB(false);
			// logger.enableI2C_Global(true);
			// if(sensors[i]->getSensorPort() > 0) { //If not a Talon
			// 	talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data ports to start for the given Talon
			// 	talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			// 	talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn on data for the given port on the Talon
			// 	bool dummy1;
			// 	bool dummy2;
			// 	// sensors[i]->begin(Time.now(), dummy1, dummy2); //DEBUG!
			// }
			if(sensors[i]->totalErrors() > 0) {
				numErrors = numErrors + sensors[i]->totalErrors(); //Increment the total error count
				if(!errors.endsWith("[")) errors = errors + ","; //Only append if not first entry
				errors = errors + sensors[i]->getErrors();
			}
			// logger.enableI2C_OB(false);
			// logger.enableI2C_Global(true);
			
			
			// if(sensors[i]->getTalonPort() > 0) talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
		}
		errors = errors + "]}}"; //Close data
	//   }
	//   for (auto &device: sensors) {
	// 	  device->getData(Time.now());
	//   }
	//   String data = aux.getData(Time.now());
	if(numErrors > 0) return errors;
	else return ""; //Return null string if no errors reported 
}

String getDataString()
{
	String data = "{\"Data\":{";
	data = data + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	data = data + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	//   for(int i = 0; i<2; i++) {
		// data = data + "\"Devices\":[" + sensors[0]->getData(Time.now()) + "," + sensors[1]->getData(Time.now()) + "]}}";
		data = data + "\"Devices\":[";
		for(int i = 0; i < numSensors; i++) {
			logger.enablePower(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon
			logger.enableData(sensors[i]->getTalonPort(), true); //Turn on kestrel port for needed Talon
			logger.enableI2C_OB(false);
			logger.enableI2C_Global(true);
			bool dummy1;
			bool dummy2;
			if(sensors[i]->getTalonPort() > 0) talons[sensors[i]->getTalonPort() - 1]->begin(0, dummy1, dummy2); //DEBUG!
			if(sensors[i]->getSensorPort() > 0) { //If not a Talon
				talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data ports to start for the given Talon
				talons[sensors[i]->getTalonPort() - 1]->disablePowerAll(); //Turn off all power ports to start for the given Talon
				talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
				talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn on data for the given port on the Talon
				// bool dummy1;
				// bool dummy2;
				// sensors[i]->begin(Time.now(), dummy1, dummy2); //DEBUG!
			}
			// logger.enableI2C_OB(false);
			// logger.enableI2C_Global(true);
			data = data + sensors[i]->getData(Time.now());
			if(i + 1 < numSensors) data = data + ","; //Only append if not last entry
			if(sensors[i]->getSensorPort() > 0) {
				talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), false); //Turn off power for the given port on the Talon
				talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), false); //Turn off data for the given port on the Talon
			}
		}
		data = data + "]}}"; //Close data
	//   }
	//   for (auto &device: sensors) {
	// 	  device->getData(Time.now());
	//   }
	//   String data = aux.getData(Time.now());
	return data;
}

String getDiagnosticString(uint8_t level)
{
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Level\":" + String(level) + ",\"Devices\":["; //Concatonate number of sensors and level 
	// leader = leader + "\"Level\":" + String(level) + ","; //Concatonate diagnostic level
	String closer = "]}}";
	String output = leader;

	// output = output + "\"Devices\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
		}
		
		// }
		// logger.enablePower(sensors[i]->getTalon(), true); //Turn on power to port
		// logger.enableData(sensors[i]->getTalon(), true); //Turn on data to port
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		// bool hasCriticalError = false;
		// bool hasError = false;

  		String diagnostic = sensors[i]->selfDiagnostic(level, Time.now());
		if(output.length() - output.lastIndexOf('\n') + diagnostic.length() + closer.length() + 1 < Kestrel::MAX_MESSAGE_LENGTH) { //Add +1 to account for comma appending, subtract any previous lines from count
			if(i > 0) output = output + ","; //Add preceeding comma if not the first entry
			output = output + diagnostic; //Append result 
			// if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
		}
		else {
			output = output + closer + "\n"; //End this packet
			output = output + leader + diagnostic; //Start a new packet and add new payload 
		}
		
	}
	output = output + "]}}"; //Close diagnostic
	return output;
	///////////////////////////////////
	// String diagnostic = "{\"Diagnostic\":{";
	// diagnostic = diagnostic + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	// diagnostic = diagnostic + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	// diagnostic = diagnostic + "\"Level\":" + String(level) + ","; //Concatonate diagnostic level
	// //   for(int i = 0; i<2; i++) {
	// 	// data = data + "\"Devices\":[" + sensors[0]->getData(Time.now()) + "," + sensors[1]->getData(Time.now()) + "]}}";
	// 	diagnostic = diagnostic + "\"Devices\":[";
	// 	for(int i = 0; i < numSensors; i++) {
	// 		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
	// 		// Serial.print("Talon Port "); //DEBUG!
	// 		// Serial.print(i);
	// 		// Serial.print(": ");
	// 		// Serial.println(sensors[i]->getTalonPort());
	// 		// Serial.flush();
	// 		logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
	// 		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
	// 		if(sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
	// 			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
	// 			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
	// 		}
			
	// 		// }
	// 		// logger.enablePower(sensors[i]->getTalon(), true); //Turn on power to port
	// 		// logger.enableData(sensors[i]->getTalon(), true); //Turn on data to port
	// 		logger.enableI2C_OB(false);
	// 		logger.enableI2C_Global(true);
	// 		diagnostic = diagnostic + sensors[i]->selfDiagnostic(level, Time.now());
	// 		if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
	// 	}
	// 	diagnostic = diagnostic + "]}}"; //Close diagnostic
	// //   }
	// //   for (auto &device: sensors) {
	// // 	  device->getData(Time.now());
	// //   }
	// //   String data = aux.getData(Time.now());
	// return diagnostic;
}

String getMetadataString()
{
	String metadata = "{\"Metadata\":{";
	metadata = metadata + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	metadata = metadata + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	metadata = metadata + "\"System\":{";
	metadata = metadata + "\"Firm\":\"" + firmwareVersion + "\",";
	metadata = metadata + "\"OS\":\"" + System.version() + "\",";
	metadata = metadata + "\"ID\":\"" + System.deviceID() + "\"},";
	//FIX! Add support for device name 
	//   for(int i = 0; i<2; i++) {
		// metadata = metadata + "\"Devices\":[" + sensors[0]->getMetadata() + "," + sensors[1]->getMetadata() + "]}}";
	metadata = metadata + "\"Devices\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
			// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
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
	//   }
	//   for (auto &device: sensors) {
	// 	  device->getData(Time.now());
	//   }
	//   String data = aux.getData(Time.now());
	return metadata;
}

String initSensors()
{
	String leader = "{\"Diagnostic\":{";
	leader = leader + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	leader = leader + "\"NumDevices\":" + String(numSensors) + ",\"Devices\":["; //Concatonate number of sensors 
	
	String closer = "]}}";
	String output = leader;

	// output = output + "\"Devices\":[";
	for(int i = 0; i < numSensors; i++) {
		logger.disableDataAll(); //Turn off data to all ports, then just enable those needed
		logger.enableData(sensors[i]->getTalonPort(), true); //Turn on data to required Talon port
		logger.enableI2C_OB(false);
		logger.enableI2C_Global(true);
		// if(!sensors[i]->isTalon()) { //If sensor is not Talon
		if(sensors[i]->getTalonPort() > 0) { //If a Talon is associated with the sensor, turn that port on
			talons[sensors[i]->getTalonPort() - 1]->disableDataAll(); //Turn off all data on Talon
			talons[sensors[i]->getTalonPort() - 1]->enablePower(sensors[i]->getSensorPort(), true); //Turn on power for the given port on the Talon
			talons[sensors[i]->getTalonPort() - 1]->enableData(sensors[i]->getSensorPort(), true); //Turn back on only port used
			
		}
		
		// }
		// logger.enablePower(sensors[i]->getTalon(), true); //Turn on power to port
		// logger.enableData(sensors[i]->getTalon(), true); //Turn on data to port
		// logger.enableI2C_OB(false);
		// logger.enableI2C_Global(true);
		bool hasCriticalError = false;
		bool hasError = false;

  		String initDiagnostic = sensors[i]->begin(Time.now(), hasCriticalError, hasError);
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
	output = output + "]}}"; //Close diagnostic
	return output;
}