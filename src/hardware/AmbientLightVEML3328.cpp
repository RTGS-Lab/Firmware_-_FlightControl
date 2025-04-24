/**
 * @file AmbientLightVEML3328.cpp
 * @brief Implementation of the AmbientLightVEML3328 class.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "AmbientLightVEML3328.h"

 AmbientLightVEML3328::AmbientLightVEML3328() : veml() {
     // Default constructor, delegates to VEML3328 constructor
 }
 
 int AmbientLightVEML3328::begin() {
     return veml.begin();
 }
 
 float AmbientLightVEML3328::getValue(Channel channel) {
     // Map from IAmbientLight::Channel to VEML3328::Channel
     VEML3328::Channel vemlChannel;
     switch (channel) {
         case Channel::Clear:
             vemlChannel = VEML3328::Channel::Clear;
             break;
         case Channel::Red:
             vemlChannel = VEML3328::Channel::Red;
             break;
         case Channel::Green:
             vemlChannel = VEML3328::Channel::Green;
             break;
         case Channel::Blue:
             vemlChannel = VEML3328::Channel::Blue;
             break;
         case Channel::IR:
             vemlChannel = VEML3328::Channel::IR;
             break;
         default:
             // Default to Clear if invalid channel
             vemlChannel = VEML3328::Channel::Clear;
             break;
     }
 
     return veml.GetValue(vemlChannel);
 }
 
 float AmbientLightVEML3328::getValue(Channel channel, bool &state) {
     // Map from IAmbientLight::Channel to VEML3328::Channel
     VEML3328::Channel vemlChannel;
     switch (channel) {
         case Channel::Clear:
             vemlChannel = VEML3328::Channel::Clear;
             break;
         case Channel::Red:
             vemlChannel = VEML3328::Channel::Red;
             break;
         case Channel::Green:
             vemlChannel = VEML3328::Channel::Green;
             break;
         case Channel::Blue:
             vemlChannel = VEML3328::Channel::Blue;
             break;
         case Channel::IR:
             vemlChannel = VEML3328::Channel::IR;
             break;
         default:
             // Default to Clear if invalid channel
             vemlChannel = VEML3328::Channel::Clear;
             break;
     }
 
     return veml.GetValue(vemlChannel, state);
 }
 
 float AmbientLightVEML3328::getLux() {
     return veml.GetLux();
 }
 
 int AmbientLightVEML3328::autoRange() {
     return veml.AutoRange();
 }