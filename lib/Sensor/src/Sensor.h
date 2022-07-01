#ifndef Sensor_h
#define Sensor_h

#include <Particle.h>

namespace BusType {
	constexpr uint8_t NONE = 0; 
	constexpr uint8_t I2C = 1;
	constexpr uint8_t SDI12 = 2;
};

class Sensor
{

	public:
		// Sensor();
		virtual String begin(time_t time, bool &criticalFault, bool &fault);
		
		virtual String getErrors() {
			return "{}"; //DEBUG!
		}
		virtual uint8_t totalErrors() {
			return 0; //DEBUG!
		}
		virtual String getData(time_t time) {
			return ""; //Return empty string if function is not implemented by device 
		};
		virtual String getMetadata() {
			return ""; //return empty string if function is not implemented by device 
		};
		virtual String selfDiagnostic(uint8_t diagnosticLevel, time_t time) {
			return ""; //return empty string if function is not implemented by device 
		};
		// virtual bool isTalon() {
		// 	return false; //Default to not
		// };
		virtual uint8_t getTalonPort() {
			return 0; //DEBUG!
		};
		virtual uint8_t getSensorPort() {
			return 0; //DEBUG!
		};
		virtual void setTalonPort(uint8_t port){};
		virtual void setSensorPort(uint8_t pos){};
		virtual bool isPresent() {
			return false;
		}; 
		uint8_t sensorInterface = BusType::NONE;
};

#endif