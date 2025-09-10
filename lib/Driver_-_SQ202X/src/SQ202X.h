//© 2023 Regents of the University of Minnesota. All rights reserved.

#ifndef SQ202X_h
#define SQ202X_h

#include <Sensor.h>
#include <SDI12Talon.h>

class SQ202X: public Sensor
{
	constexpr static int DEAFULT_PORT = 3; ///<Use port 3 by default (SDI12Talon default)
	constexpr static int DEFAULT_SENSOR_PORT = 4; ///<Use port 4 by default (Apogee port)
  	constexpr static int DEFAULT_VERSION = 0x00; ///<Use hardware version unknown by default
	const String FIRMWARE_VERSION = "0.1.0"; //FIX! Read from system??
  	constexpr static int MAX_NUM_ERRORS = 10; ///<Maximum number of errors to log before overwriting previous errors in buffer


	public:
		SQ202X(SDI12Talon& talon_, uint8_t talonPort_ = DEAFULT_PORT, uint8_t sensorPort_ = DEFAULT_SENSOR_PORT, uint8_t version = DEFAULT_VERSION);
		String begin(time_t time, bool &criticalFault, bool &fault);
		String getData(time_t time);
		String selfDiagnostic(uint8_t diagnosticLevel = 4, time_t time = 0); //Default to just level 4 diagnostic, default to time = 0
		String getMetadata();
		String getErrors();
		bool isPresent();

	private:
		SDI12Talon& talon;
		bool initDone = false; //Used to keep track if the initaliztion has run - used by hasReset() 
		uint8_t version = 0; //FIX! This should be read from EEPROM in future 

		// SQ-202X Specific Calibration Parameters
		const float MV_TO_PPFD_SCALE = 1.6; // 1.6 µmol m⁻² s⁻¹ per mV (SQ-202X calibration factor)
		const float MV_TO_PPFD_OFFSET = 0.0; // No offset for linear sensor
		const String SENSOR_UNITS = "umol_m2_s"; // µmol m⁻² s⁻¹ (PPFD units)
		const String SENSOR_TYPE = "SQ-202X"; // Apogee SQ-202X Quantum Sensor
		const String SENSOR_DESCRIPTION = "Apogee SQ-202X Quantum Sensor";
};

#endif