#ifndef Sensor_h
#define Sensor_h

class Sensor
{

	public:
		// Sensor();
		virtual String begin(time_t time, bool &criticalFault, bool &fault);
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
};

#endif