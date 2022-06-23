#ifndef Talon_h
#define Talon_h

class Talon
{
	public:
		// Talon();
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
		virtual uint8_t getTalonPort() {
			return 0; //DEBUG!
		};
		virtual void setTalonPort(uint8_t port_);
		virtual bool isPresent();
		// virtual uint8_t getPort();
		// virtual void setPort(uint8_t port);

};

#endif