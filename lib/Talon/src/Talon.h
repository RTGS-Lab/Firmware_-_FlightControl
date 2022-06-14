#ifndef Talon_h
#define Talon_h

class Talon
{
	public:
		// Talon();
		int restart();
		virtual int enableData(uint8_t Port){
			return 0;
		};
		virtual int enablePower(uint8_t Port){
			return 0;
		};

};

#endif