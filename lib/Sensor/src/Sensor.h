#ifndef Sensor_h
#define Sensor_h

class Sensor
{

	public:
		// Sensor();
		virtual String getData(time_t time) {
			return ""; //Return empty string if function is not implemented by device 
		};
		virtual String getMetadata() {
			return ""; //return empty string if function is not implemented by device 
		};
		virtual String selfDiagnostic(uint8_t diagnosticLevel, time_t time) {
			return ""; //return empty string if function is not implemented by device 
		};
};

#endif