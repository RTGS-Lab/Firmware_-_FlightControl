//© 2025 Regents of the University of Minnesota. All rights reserved.

#ifndef SDI12ANALOGMUX_h
#define SDI12ANALOGMUX_h

#include <Sensor.h>
#include <SDI12Talon.h>

class SDI12AnalogMux: public Sensor
{
	constexpr static int DEFAULT_PORT = 2; ///<Use port 2 by default
	constexpr static int DEFAULT_SENSOR_PORT = 0; ///<Use port 0 by default
	constexpr static int DEFAULT_VERSION = 0x00; ///<Use hardware version unknown by default
	const String FIRMWARE_VERSION = "1.0.0";

	public:
		SDI12AnalogMux(SDI12Talon& talon_, uint8_t talonPort_ = DEFAULT_PORT, uint8_t sensorPort_ = DEFAULT_SENSOR_PORT, uint8_t version = DEFAULT_VERSION);
		String begin(time_t time, bool &criticalFault, bool &fault);
		String getData(time_t time);
		String selfDiagnostic(uint8_t diagnosticLevel = 4, time_t time = 0);
		String getMetadata();
		String getErrors();
		bool isPresent();

	private:
		SDI12Talon& talon;
		String appendData(float data, String label, uint8_t precision = 2, bool appendComma = true);
		bool parseTemperature(String input, float &temperature);

		bool initDone = false;
		uint8_t version = 0;
};

#endif
