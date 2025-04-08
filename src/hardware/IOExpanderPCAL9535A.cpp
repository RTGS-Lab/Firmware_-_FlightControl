/******************************************************************************
 * IOExpanderPCAL9535A.cpp
 * Concrete implementation of IIOExpander for the PCAL9535A chip.
 * Delegates calls to the PCAL9535A_Library driver.
 *
 *
 * Distributed as-is; no warranty is given.
 ******************************************************************************/

 #include "IOExpanderPCAL9535A.h"

 // Include standard types/constants if not guaranteed by includes above
 // (e.g., if IIOExpander.h doesn't define INPUT, OUTPUT etc.)
 // #include <Arduino.h> // Or <Particle.h>
 
 // --- Constructor ---
 IOExpanderPCAL9535A::IOExpanderPCAL9535A(int address)
     : pcal9535a(address) // Initialize the driver instance with the address
 {
     // Constructor body (if needed)
 }
 
 // --- IIOExpander Interface Method Implementations ---
 
 int IOExpanderPCAL9535A::begin() {
     // Address is typically set in the constructor for I2C devices.
     // The driver's begin() usually initializes Wire and checks presence.
     return pcal9535a.begin();
 }
 
 // --- Core IO ---
 int IOExpanderPCAL9535A::pinMode(int Pin, uint8_t State, bool Port) {
     return pcal9535a.pinMode(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::pinMode(int Pin, uint8_t State) {
     return pcal9535a.pinMode(Pin, State);
 }
 
 int IOExpanderPCAL9535A::digitalWrite(int Pin, bool State, bool Port) {
     return pcal9535a.digitalWrite(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::digitalWrite(int Pin, bool State) {
     return pcal9535a.digitalWrite(Pin, State);
 }
 
 int IOExpanderPCAL9535A::digitalRead(int Pin, bool Port) {
     return pcal9535a.digitalRead(Pin, Port);
 }
 
 int IOExpanderPCAL9535A::digitalRead(int Pin) {
     return pcal9535a.digitalRead(Pin);
 }
 
 // --- Specific Features ---
 

 int IOExpanderPCAL9535A::pinSetDriveStrength(int Pin, DriveStrength State, bool Port) {
     return pcal9535a.pinSetDriveStrength(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::pinSetDriveStrength(int Pin, DriveStrength State) {
     return pcal9535a.pinSetDriveStrength(Pin, State);
 }
 
 
 int IOExpanderPCAL9535A::setInterrupt(int Pin, bool State, bool Port) {
     return pcal9535a.setInterrupt(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::setInterrupt(int Pin, bool State) {
     return pcal9535a.setInterrupt(Pin, State);
 }
 
 int IOExpanderPCAL9535A::getInterrupt(int Pin) {
     return pcal9535a.getInterrupt(Pin);
 }
 
 uint16_t IOExpanderPCAL9535A::getAllInterrupts(uint8_t Option) {
     // Delegate using the mapped age/status value
     return pcal9535a.getAllInterrupts(Option);
 }
 
 uint16_t IOExpanderPCAL9535A::getInterruptMask() {
     return pcal9535a.getInterruptMask();
 }
 
 unsigned int IOExpanderPCAL9535A::clearInterrupt(uint8_t age) {
     // Delegate using the mapped age/status value
     return pcal9535a.clearInterrupt(age);
 }
 
 bool IOExpanderPCAL9535A::isInterrupt(uint8_t age) {
     // Driver doesn't have direct isInterrupt, implement via getAllInterrupts
     return (pcal9535a.getAllInterrupts(age) != 0);
 }
 
 int IOExpanderPCAL9535A::setLatch(int Pin, bool State, bool Port) {
     return pcal9535a.setLatch(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::setLatch(int Pin, bool State) {
     return pcal9535a.setLatch(Pin, State);
 }
 
 uint16_t IOExpanderPCAL9535A::getLatch() {
     return pcal9535a.getLatch();
 }
 
 int IOExpanderPCAL9535A::setInputPolarity(int Pin, bool State, bool Port) {
     return pcal9535a.setInputPolarity(Pin, State, Port);
 }
 
 int IOExpanderPCAL9535A::setInputPolarity(int Pin, bool State) {
     return pcal9535a.setInputPolarity(Pin, State);
 }
 
 bool IOExpanderPCAL9535A::getInputPolarity(int Pin, bool Port) {
     // Driver returns int (-1 error, 0/1 state), interface expects bool
     int result = pcal9535a.getInputPolarity(Pin, Port);
     // Consider result >= 0 for true if 0 is valid, or just result == 1?
     // Assuming 1 means "polarity inverted enabled", 0 means "disabled"
     return (result == 1); // Adjust if 0 means true for your interface
 }
 
 bool IOExpanderPCAL9535A::getInputPolarity(int Pin) {
     int result = pcal9535a.getInputPolarity(Pin);
     return (result == 1); // Adjust as needed
 }
 
 int IOExpanderPCAL9535A::setBusOutput(uint8_t mode, bool Port) {
     // Assumes interface OutputType constants match driver expectations
     return pcal9535a.setBusOutput(mode, Port);
 }
 
 uint8_t IOExpanderPCAL9535A::getBusOutput() {
    return 0; //not implemented in driver
 }
 
 uint16_t IOExpanderPCAL9535A::readBus() {
     return pcal9535a.readBus();
 }
 
 // --- Error handling ---
 uint16_t IOExpanderPCAL9535A::getError() {
     return pcal9535a.getError();
 }
 
 uint16_t IOExpanderPCAL9535A::clearError() {
     return pcal9535a.clearError();
 }
 
 void IOExpanderPCAL9535A::safeMode(int state) {
     // Assumes interface constants match driver's SAFE, SAFE1, etc.
     pcal9535a.safeMode(state);
 }

 uint16_t IOExpanderPCAL9535A::readWord(int Pos, int &Error) {
    return pcal9535a.readWord(Pos, Error);
 }

 int IOExpanderPCAL9535A::setIntPinConfig(int Pin, bool Latch) {
    return pcal9535a.setIntPinConfig(Pin, Latch);
}