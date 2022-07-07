#ifndef Talon_h
#define Talon_h

#include <Sensor.h>

class Talon: public Sensor
{
	public:
		// Talon();
		virtual String begin(time_t time, bool &criticalFault, bool &fault);
		int restart();
		virtual int enableData(uint8_t Port, bool state){
			return 0;
		};
		virtual int enablePower(uint8_t Port, bool state){
			return 0;
		};
		int disableDataAll(){
			// for(int i = 0; i < 4; i++) {
			// 	enableData(i, false);
			// }
			return 0;
		};
		int disablePowerAll(){
			// for(int i = 0; i < numPorts; i++) {
			// 	enablePower(i, false);
			// }
			return 0;
		};
		// uint8_t getPort() {
		// 	if(talonPort < 255) return talonPort + 1;
		// 	else return 0;
		// }
		// void setPort(uint8_t port)
		// {
		// 	// // if(port_ > numPorts || port_ == 0) throwError(PORT_RANGE_ERROR | portErrorCode); //If commanded value is out of range, throw error 
		// 	// if(port > 4 || port == 0) throwError(TALON_PORT_RANGE_ERROR | talonPortErrorCode | sensorPortErrorCode); //If commanded value is out of range, throw error //FIX! How to deal with magic number? This is the number of ports on KESTREL, how do we know that??
		// 	// else { //If in range, update the port values
		// 	// 	talonPort = port - 1; //Set global port value in index counting
		// 	// 	talonPortErrorCode = (talonPort + 1) << 4; //Set port error code in rational counting 
		// 	// }
			
		// }
		// // virtual void setPort(uint8_t port_);
		// virtual bool isPresent();
	// protected:
		virtual uint8_t getNumPorts() {
			return 0; //DEBUG!
		};
		uint8_t talonInterface = 0;
	protected:
		// const uint32_t TALON_PORT_RANGE_ERROR = 0x90010200; //FIX! 
		// uint8_t sensorInterface = BusType::NONE;
		// uint8_t talonPort = 255; //Used to keep track of which port the Talon is connected to on Kestrel
		// uint8_t sensorPort = 255; //Used to keep track of which port the sensor is connected to on associated Talon
		// virtual uint8_t getPort();
		// virtual void setPort(uint8_t port);

};

#endif