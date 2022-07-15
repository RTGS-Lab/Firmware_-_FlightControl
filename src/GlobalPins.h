#ifndef GlobalPins_h
#define GlobalPins_h

// namespace KestrelPins { //Use for B402
// 	constexpr uint16_t WD_HOLD  = D2;
// 	constexpr uint16_t SD_CS    = D8;
// 	constexpr uint16_t Clock_INT 	= D22;
// 	constexpr uint16_t TALON1_GPIOA = A3;
// 	constexpr uint16_t TALON1_GPIOB = D7;
// 	constexpr uint16_t TALON2_GPIOA = A2;
// 	constexpr uint16_t TALON2_GPIOB = D6;
// 	constexpr uint16_t TALON3_GPIOA = A1;
// 	constexpr uint16_t TALON3_GPIOB = D5;
// 	constexpr uint16_t I2C_GLOBAL_EN = D23; //FIX!
// 	constexpr uint16_t I2C_OB_EN = A6; //FIX!
// }
namespace KestrelPins {
	constexpr uint8_t PortAPins[4] = {A3, A2, A1, A0};
	constexpr uint8_t PortBPins[4] = {D7, D6, D5, D4};
	constexpr uint16_t I2C_OB_EN = A6;
	constexpr uint16_t I2C_GLOBAL_EN = D23;
}

// class KestrelPins
// {
// 	public:
// 		// Talon();
// 		// const uint8_t PortAPins[4] = {A3, A2, A1, A0};
// 		// const uint8_t PortBPins[4] = {D7, D6, D5, D4};
// 		// int restart();
// 		// virtual int enableData(uint8_t Port){
// 		// 	return 0;
// 		// };
// 		// virtual int enablePower(uint8_t Port){
// 		// 	return 0;
// 		// };

// };

#endif