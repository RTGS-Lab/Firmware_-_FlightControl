//© 2025 Regents of the University of Minnesota. All rights reserved.

#include <SDI12AnalogMux.h>

SDI12AnalogMux::SDI12AnalogMux(SDI12Talon& talon_, uint8_t talonPort_, uint8_t sensorPort_, uint8_t version): talon(talon_)
{
	//Only update values if they are in range, otherwise stick with default values
	if(talonPort_ > 0) talonPort = talonPort_ - 1;
	else talonPort = 255; //Reset to null default if not in range
	if(sensorPort_ > 0) sensorPort = sensorPort_ - 1;
	else sensorPort = 255; //Reset to null default if not in range
	sensorInterface = BusType::SDI12;
}

String SDI12AnalogMux::begin(time_t time, bool &criticalFault, bool &fault)
{
	return ""; //No special initialization required
}

String SDI12AnalogMux::selfDiagnostic(uint8_t diagnosticLevel, time_t time)
{
	if(getSensorPort() == 0) throwError(FIND_FAIL); //If no port found, report failure
	else if(isPresent() == false) throwError(DETECT_FAIL); //If sensor port is good, but fail to detect sensor, throw error
	String output = "\"SDI12AnalogMux\":{";
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
		//TBD
	}

	if(diagnosticLevel <= 5) {
		if(getSensorPort() != 0 && isPresent() == true) { //Test as normal
			String adr = talon.sendCommand("?!");
			int adrVal = adr.toInt();
			output = output + "\"Adr\":";
			if(adr.equals("") || (!adr.equals("0") && adrVal == 0)) output = output + "null"; //If no return, report null
			else output = output + adr; //Otherwise report the read value
			output = output + ",";
		}
		else output = output + "\"Adr\":null,"; //Else append null string

	}
	return output + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]}"; //Write position in logical form - Return completed closed output
}

String SDI12AnalogMux::getMetadata()
{
	uint8_t adr = (talon.sendCommand("?!")).toInt(); //Get address of local device
	String id = talon.command("I", adr);
	Serial.println(id); //DEBUG!
	String sdi12Version;
	String mfg;
	String model;
	String senseVersion;
	String sn;
	if((id.substring(0, 1)).toInt() != adr) { //If address returned is not the same as the address read, throw error
		Serial.println("ADDRESS MISMATCH!"); //DEBUG!
		//Throw error!
		sdi12Version = "null";
		mfg = "null";
		model = "null";
		senseVersion = "null";
		sn = "null";
	}
	else { //Standard across SDI-12 devices
		sdi12Version = (id.substring(1,3)).trim(); //Grab SDI-12 version code
		mfg = (id.substring(3, 11)).trim(); //Grab manufacturer
		model = (id.substring(11,17)).trim(); //Grab sensor model name
		senseVersion = (id.substring(17,20)).trim(); //Grab version number
		sn = (id.substring(20,33)).trim(); //Grab the serial number
	}
	String metadata = "\"SDI12AnalogMux\":{";
	metadata = metadata + "\"Hardware\":\"" + senseVersion + "\","; //Report sensor version pulled from SDI-12 system
	metadata = metadata + "\"Firmware\":\"" + FIRMWARE_VERSION + "\","; //Static firmware version
	metadata = metadata + "\"SDI12_Ver\":\"" + sdi12Version.substring(0,1) + "." + sdi12Version.substring(1,2) + "\",";
	metadata = metadata + "\"ADR\":" + String(adr) + ",";
	metadata = metadata + "\"Mfg\":\"" + mfg + "\",";
	metadata = metadata + "\"Model\":\"" + model + "\",";
	metadata = metadata + "\"SN\":\"" + sn + "\",";
	metadata = metadata + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]"; //Concatenate position
	metadata = metadata + "}"; //CLOSE
	return metadata;
}

String SDI12AnalogMux::getData(time_t time)
{
	String output = "\"SDI12AnalogMux\":{"; //OPEN JSON BLOB
	bool readDone = false;
	delay(2500); //Give sensor time to initialize (ADC + RTD config + SDI-12 setup)

	// Arrays to store temperature readings for all 9 channels
	float temperatures[9] = {-9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999}; //Initialize with error values
	bool channelSuccess[9] = {false, false, false, false, false, false, false, false, false};

	if(getSensorPort() != 0) { //Check for valid port
		int adr = -1;

		// First, get the sensor address (only need to do this once)
		for(int retry = 0; retry < talon.retryCount; retry++) {
			if(!isPresent()) continue; //If presence check fails, try again
			adr = talon.getAddress();
			if(adr >= 0) break; //Address found successfully
		}

		if(adr >= 0) {
			// Read all 9 temperature channels (M1-M9)
			for(int channel = 1; channel <= 9; channel++) {
				for(int retry = 0; retry < talon.retryCount; retry++) {
					// Send measurement command (aM1! through aM9!)
					int waitTime = talon.startMeasurmentIndex(channel, adr);
					if(waitTime < 0) {
						continue; //If wait time invalid, try again
					}

					// Wait for measurement to complete (sensor returns 0 seconds, but add small buffer)
					delay(waitTime*1000 + 100); //Wait requested time plus 100ms buffer

					// Retrieve data with D0 command
					String data = talon.command("D0", adr);

					// Parse the temperature value
					if(parseTemperature(data, temperatures[channel-1])) {
						channelSuccess[channel-1] = true;
						readDone = true;
						break; //Successfully read this channel, move to next
					}
				}
			}
		}

		if(readDone == false) throwError(talon.SDI12_READ_FAIL); //Throw error if no channels read successfully
	}
	else throwError(FIND_FAIL);

	// Build JSON output with all 9 channels
	output = output + appendData(temperatures[0], "RTD1_Temp", 2, true);
	output = output + appendData(temperatures[1], "RTD2_Temp", 2, true);
	output = output + appendData(temperatures[2], "RTD3_Temp", 2, true);
	output = output + appendData(temperatures[3], "RTD4_Temp", 2, true);
	output = output + appendData(temperatures[4], "RTD5_Temp", 2, true);
	output = output + appendData(temperatures[5], "RTD6_Temp", 2, true);
	output = output + appendData(temperatures[6], "RTD7_Temp", 2, true);
	output = output + appendData(temperatures[7], "Pico_Temp", 2, true);
	output = output + appendData(temperatures[8], "ADC_Temp", 2, false); //Last entry, no trailing comma

	output = output + ",\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]"; //Concatenate position
	output = output + "}"; //CLOSE JSON BLOB
	Serial.println(output); //DEBUG!
	return output;
}

bool SDI12AnalogMux::isPresent()
{
	uint8_t adr = (talon.sendCommand("?!")).toInt();

	String id = talon.command("I", adr);
	id.remove(0, 1); //Trim address character from start
	Serial.print("SDI12 Address: "); //DEBUG!
	Serial.print(adr);
	Serial.print(",");
	Serial.println(id);
	// Check for either "GEMS" (vendor) or "GORGON" (model) in identification string
	if(id.indexOf("GEMS") > 0 || id.indexOf("GORGON") > 0) return true;
	else return false;
}

String SDI12AnalogMux::appendData(float data, String label, uint8_t precision, bool appendComma)
{
	String val = "";
	if(data == -9999 || data == 9999999) val = "\"" + label + "\":null"; //Append null if value is error indicator
	else val = "\"" + label + "\":" + String(data, precision); //Otherwise, append as normal using fixed specified precision

	if(appendComma) return val + ",";
	else return val;
}

bool SDI12AnalogMux::parseTemperature(String input, float &temperature)
{
	// Expected format: "a±xx.xx<CR><LF>" where a is address
	// Example: "0+25.43\r\n"

	if(input.length() < 3) return false; //String too short to be valid

	// Remove address character from start
	input.remove(0, 1);
	input.trim(); //Remove any trailing whitespace/CR/LF

	// Check if string starts with + or - sign
	if(input.charAt(0) != '+' && input.charAt(0) != '-') return false;

	// Parse the float value
	temperature = input.toFloat();

	// Basic sanity check: PT100 sensors typically measure -200°C to +850°C
	// But for this application, reasonable range is likely -50 to +150°C
	if(temperature < -200.0 || temperature > 1000.0) {
		temperature = -9999; //Set to error value
		return false;
	}

	return true;
}

String SDI12AnalogMux::getErrors()
{
	String output = "\"SDI12AnalogMux\":{"; // OPEN JSON BLOB
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
