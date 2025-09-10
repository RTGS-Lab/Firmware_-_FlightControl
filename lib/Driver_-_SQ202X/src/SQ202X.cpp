//© 2023 Regents of the University of Minnesota. All rights reserved.

#include <SQ202X.h>

SQ202X::SQ202X(SDI12Talon& talon_, uint8_t talonPort_, uint8_t sensorPort_, uint8_t version): talon(talon_)
{
	//Only update values if they are in range, otherwise stick with default values
	if(talonPort_ > 0) talonPort = talonPort_ - 1;
	else talonPort = 255; //Reset to null default if not in range
	if(sensorPort_ > 0) sensorPort = sensorPort_ - 1;
	else sensorPort = 255; //Reset to null default if not in range 
	sensorInterface = BusType::ANALOG; 
}

String SQ202X::begin(time_t time, bool &criticalFault, bool &fault)
{
	// Check if SDI-12 is detected on the Apogee port - if so, this is a conflict
	if(talon.isApogeeDetected() == true) {
		fault = true; //Set fault flag - analog sensor cannot work if SDI-12 is detected
		throwError(DETECT_FAIL | talonPortErrorCode | sensorPortErrorCode); 
		return ""; //Return early - cannot initialize analog sensor if SDI-12 is active
	}
	
	return selfDiagnostic(5, time); //Run diagnostic to verify analog functionality
}

String SQ202X::selfDiagnostic(uint8_t diagnosticLevel, time_t time)
{
	if(getSensorPort() == 0) throwError(FIND_FAIL); //If no port found, report failure
	else if(isPresent() == false) throwError(DETECT_FAIL); //If sensor port is good, but fail to detect sensor, throw error 
	String output = "\"Apogee SQ-202X\":{";
	
	if(diagnosticLevel == 0) {
		//TBD
	}

	if(diagnosticLevel <= 1) {
		//TBD
	}

	if(diagnosticLevel <= 2) {
		//TBD
	}

	if(diagnosticLevel <= 3) {
		//TBD
 	}

	if(diagnosticLevel <= 4) {
		if(getSensorPort() != 0 && isPresent() == true) { //Test as normal
			// Get analog voltage - getApogeeVoltage handles port configuration internally
			float voltage = talon.getApogeeVoltage(5.0);
			float voltageMillivolts = voltage * 1000.0; // Convert V to mV
			float ppfd = voltageMillivolts * MV_TO_PPFD_SCALE + MV_TO_PPFD_OFFSET;
			output = output + "\"PPFD\":" + String(ppfd) + ",";
			output = output + "\"Voltage\":" + String(voltage) + ",";
			output = output + "\"Voltage_mV\":" + String(voltageMillivolts) + ",";
		}
		else output = output + "\"PPFD\":null,\"Voltage\":null,\"Voltage_mV\":null,"; //Otherwise append null string
	}

	if(diagnosticLevel <= 5) {
		output = output + "\"Type\":\"" + SENSOR_TYPE + "\",";
		output = output + "\"Units\":\"" + SENSOR_UNITS + "\",";
	}
	
	return output + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]}"; //Write position in logical form - Return completed closed output
}

String SQ202X::getMetadata()
{
	String metadata = "\"Apogee SQ-202X\":{";
	metadata = metadata + "\"Description\":\"" + SENSOR_DESCRIPTION + "\",";
	metadata = metadata + "\"Hardware\":\"Unknown\","; //No direct hardware version available from analog interface
	metadata = metadata + "\"Firmware\":\"" + FIRMWARE_VERSION + "\","; //Static firmware version 
	metadata = metadata + "\"Type\":\"" + SENSOR_TYPE + "\",";
	metadata = metadata + "\"Units\":\"" + SENSOR_UNITS + "\",";
	metadata = metadata + "\"Calibration\":\"" + String(MV_TO_PPFD_SCALE) + " umol m-2 s-1 per mV\",";
	metadata = metadata + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]"; //Concatenate position 
	metadata = metadata + "}"; //CLOSE  
	return metadata; 
}

String SQ202X::getData(time_t time)
{
	String output = "\"Apogee SQ-202X\":{"; //OPEN JSON BLOB
	bool readDone = false;
	
	if(getSensorPort() != 0) { //Check both for detection 
		for(int i = 0; i < talon.retryCount; i++) {
			if(!isPresent()) continue; //If presence check fails, try again

			// Get analog voltage - getApogeeVoltage handles port configuration internally
			float voltage = talon.getApogeeVoltage(5.0);
			
			// Convert voltage to PPFD using SQ-202X calibration (1.6 µmol m⁻² s⁻¹ per mV)
			float voltageMillivolts = voltage * 1000.0; // Convert V to mV
			float ppfd = voltageMillivolts * MV_TO_PPFD_SCALE + MV_TO_PPFD_OFFSET;
			
			output = output + "\"PPFD\":" + String(ppfd) + ",\"Voltage\":" + String(voltage) + ",\"Voltage_mV\":" + String(voltageMillivolts); //Concatenate data
			readDone = true; //Set flag
			break; //Stop retry if successful 
		}
	}
	
	if(getSensorPort() == 0 || readDone == false) output = output + "\"PPFD\":null,\"Voltage\":null,\"Voltage_mV\":null"; //Otherwise generate null report 
	if(readDone == false) throwError(SENSOR_TIMEOUT | talonPortErrorCode | sensorPortErrorCode);
	
	output = output + ",\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]"; //Concatenate position 
	output = output + "}"; //CLOSE JSON BLOB
	return output;
}

bool SQ202X::isPresent() 
{
	// For analog sensors, we consider it "present" if:
	// 1. We have a valid sensor port (port 4)
	// 2. SDI-12 is NOT detected on the Apogee port (analog mode)
	// 3. We can get a reasonable voltage reading (not completely out of range)
	
	if(getSensorPort() != 4) return false; //Must be on port 4 for Apogee
	if(talon.isApogeeDetected() == true) return false; //Cannot be present if SDI-12 is detected
	
	// Try to get a voltage reading - if it's completely out of range, sensor may not be connected
	float voltage = talon.getApogeeVoltage(5.0); // getApogeeVoltage handles port configuration
	
	// Basic sanity check - voltage should be within reasonable range (0-5V)
	if(voltage < 0 || voltage > 5.5) return false; //Outside reasonable range
	
	return true; //Sensor appears to be present and responding
}

String SQ202X::getErrors()
{
	String output = "\"Apogee SQ-202X\":{"; // OPEN JSON BLOB
	output = output + "\"CODES\":["; //Open codes pair

	for(int i = 0; i < min(MAX_NUM_ERRORS, numErrors); i++) { //Iterate over used element of array without exceeding bounds
		output = output + "\"0x" + String(errors[i], HEX) + "\","; //Add each error code
		errors[i] = 0; //Clear errors as they are read
	}
	if(output.substring(output.length() - 1).equals(",")) {
		output = output.substring(0, output.length() - 1); //Trim trailing ','
	}
	output = output + "],"; //close codes pair
	output =  output + "\"OW\":"; //Open state pair
	if(numErrors > MAX_NUM_ERRORS) output = output + "1,"; //If overwritten, indicate the overwrite is true
	else output = output + "0,"; //Otherwise set it as clear
	output = output + "\"NUM\":" + String(numErrors) + ","; //Append number of errors
	output = output + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]"; //Concatenate position 
	output = output + "}"; //CLOSE JSON BLOB
	numErrors = 0; //Clear error count
	return output;
}