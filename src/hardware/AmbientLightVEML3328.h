/**
 * @file AmbientLightVEML3328.h
 * @brief Concrete implementation of IAmbientLight using VEML3328
 *
 * Adapts the VEML3328 ambient light sensor to the IAmbientLight interface
 * for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef AMBIENT_LIGHT_VEML3328_H
 #define AMBIENT_LIGHT_VEML3328_H
 
 #include "IAmbientLight.h"
 #include "VEML3328.h" // Include the actual VEML3328 library
 
 /**
  * @brief Concrete implementation of IAmbientLight using VEML3328
  */
 class AmbientLightVEML3328 : public IAmbientLight {
 public:
     AmbientLightVEML3328();
     ~AmbientLightVEML3328() override = default;
 
     int begin() override;
     float getValue(Channel channel) override;
     float getValue(Channel channel, bool &state) override;
     float getLux() override;
     int autoRange() override;
 
 private:
     VEML3328 veml; // The concrete VEML3328 instance
 };
 
 #endif // AMBIENT_LIGHT_VEML3328_H