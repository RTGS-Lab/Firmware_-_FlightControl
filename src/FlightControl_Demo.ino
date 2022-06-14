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
#include <vector>
#include <memory>

const String firmwareVersion = "1.0.1";

AuxTalon aux(4, 0x14); //Instantiate AUX talon with deaults - port 4 and hardware v1.4
AuxTalon aux1(3, 0x14); //Instantiate AUX talon with alt - port 3 and hardware v1.4
PCAL9535A ioAlpha(0x20);
PCAL9535A ioBeta(0x21);

const uint8_t numSensors = 2; 
// std::vector<std::unique_ptr<Sensor>> sensors;
Sensor* const sensors[numSensors] = {
	&aux,
	&aux1
};
// sensors.push_back(aux);
// sensors.push_back(aux1);
// sensors.emlace_back(aux);
// sensors.emlace_back(aux1);

// Sensor sensors[2] = {aux, aux1};

namespace Pins { //Use for B402
	constexpr uint16_t WD_HOLD  = D2;
	constexpr uint16_t SD_CS    = D8;
	constexpr uint16_t Clock_INT 	= D22;
	constexpr uint16_t TALON1_GPIOA = A3;
	constexpr uint16_t TALON1_GPIOB = D7;
	constexpr uint16_t TALON2_GPIOA = A2;
	constexpr uint16_t TALON2_GPIOB = D6;
	constexpr uint16_t TALON3_GPIOA = A1;
	constexpr uint16_t TALON3_GPIOB = D5;
	constexpr uint16_t I2C_GLOBAL_EN = D23; //FIX!
	constexpr uint16_t I2C_OB_EN = A6; //FIX!
}

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
  // Put initialization like pinMode and begin functions here.
  Serial.begin(115200); 
  I2C_OnBoardEn(true); 
	Wire.setClock(400000); //Confirm operation in fast mode
	Wire.begin();
	ioAlpha.begin(); //RESTORE
	ioBeta.begin(); //RESTORE

  I2C_OnBoardEn(true); //Turn on internal I2C
  ioBeta.pinMode(PinsIOBeta::EN4, OUTPUT);
  ioBeta.pinMode(PinsIOBeta::I2C_EN4, OUTPUT);
  ioBeta.pinMode(PinsIOBeta::SEL4, OUTPUT);

  ioBeta.digitalWrite(PinsIOBeta::EN4, HIGH); //Turn on power to port 4
  ioBeta.digitalWrite(PinsIOBeta::I2C_EN4, HIGH); //Turn on I2C to port 4
  ioBeta.digitalWrite(PinsIOBeta::SEL4, LOW); //Turn on GPIO connection to port 4
  ////////// ADD INTERRUPTS!

  I2C_OnBoardEn(false);
  I2C_GlobalEn(true);

	bool hasCriticalError = false;
	bool hasError = false;
  String initDiagnostic = aux.begin(Time.now(), hasCriticalError, hasError);
	Serial.print("DIAGNOSTIC: ");
	Serial.println(initDiagnostic);
	Particle.publish("DIAGNOSTIC", initDiagnostic);
  //FIX! RESPOND TO ERROR RESULTS! 
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  
  // aux.sleep(false);
  String diagnostic = getDiagnosticString(4);
  aux.restart();
  
  String errors = aux.getErrors();
  String metadata = getMetadataString();
	String data = getDataString();

  Particle.publish("DIAGNOSTIC", diagnostic);
  Particle.publish("ERROR", errors);
  Particle.publish("DATA", data);
  Particle.publish("METADATA", metadata);

  Serial.print("DIAGNOSTIC: ");
  Serial.println(diagnostic);
  Serial.print("ERROR: ");
  Serial.println(errors);
  Serial.print("DATA: ");
  Serial.println(data);
  Serial.print("METADATA: ");
  Serial.println(metadata);
  // aux.sleep(true);
  delay(10000); //Wait a while //DEBUG!


}

void I2C_GlobalEn(bool State)
{
	pinMode(Pins::I2C_GLOBAL_EN, OUTPUT);
	digitalWrite(Pins::I2C_GLOBAL_EN, State);
}

void I2C_OnBoardEn(bool State)
{
	pinMode(Pins::I2C_OB_EN, OUTPUT);
	digitalWrite(Pins::I2C_OB_EN, State);
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
			data = data + sensors[i]->getData(Time.now());
			if(i + 1 < numSensors) data = data + ","; //Only append if not last entry
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
	String diagnostic = "{\"Diagnostic\":{";
	diagnostic = diagnostic + "\"Time\":" + String(Time.now()) + ","; //Concatonate time
	diagnostic = diagnostic + "\"NumDevices\":" + String(numSensors) + ","; //Concatonate number of sensors 
	diagnostic = diagnostic + "\"Level\":" + String(level) + ","; //Concatonate diagnostic level
	//   for(int i = 0; i<2; i++) {
		// data = data + "\"Devices\":[" + sensors[0]->getData(Time.now()) + "," + sensors[1]->getData(Time.now()) + "]}}";
		diagnostic = diagnostic + "\"Devices\":[";
		for(int i = 0; i < numSensors; i++) {
			diagnostic = diagnostic + sensors[i]->selfDiagnostic(level, Time.now());
			if(i + 1 < numSensors) diagnostic = diagnostic + ","; //Only append if not last entry
		}
		diagnostic = diagnostic + "]}}"; //Close diagnostic
	//   }
	//   for (auto &device: sensors) {
	// 	  device->getData(Time.now());
	//   }
	//   String data = aux.getData(Time.now());
	return diagnostic;
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