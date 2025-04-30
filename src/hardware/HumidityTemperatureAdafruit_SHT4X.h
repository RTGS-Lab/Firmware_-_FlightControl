/**
 * @file HumidityTemperatureAdafruit_SHT4X.h
 * @brief Concrete implementation of IHumidityTemperature using Adafruit_SHT4x
 *
 * Adapts the Adafruit SHT4x humidity and temperature sensor to the IHumidityTemperature
 * interface for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef HUMIDITY_TEMPERATURE_ADAFRUIT_SHT4X_H
 #define HUMIDITY_TEMPERATURE_ADAFRUIT_SHT4X_H
 
 #include "IHumidityTemperature.h"
 #include <Adafruit_SHT4x.h>
 
 /**
  * @brief Concrete implementation of IHumidityTemperature using Adafruit_SHT4x
  */
 class HumidityTemperatureAdafruit_SHT4X : public IHumidityTemperature {
 public:
     /**
      * @brief Constructor
      */
     HumidityTemperatureAdafruit_SHT4X();
     
     /**
      * @brief Destructor
      */
     ~HumidityTemperatureAdafruit_SHT4X() override = default;
 
     /**
      * @brief Initialize the sensor
      * @return true on success, false on failure
      */
     bool begin() override;
 
     /**
      * @brief Sets the precision level for measurements
      * @param prec The desired precision setting
      */
     void setPrecision(Iht_precision_t prec) override;
 
     /**
      * @brief Gets the current precision setting
      * @return Current precision setting
      */
     Iht_precision_t getPrecision() override;
 
     /**
      * @brief Get humidity and temperature values as sensor events
      * @param humidity Event object to be populated with humidity data (can be NULL)
      * @param temp Event object to be populated with temperature data (can be NULL)
      * @return true if read was successful
      */
     bool getEvent(Isensors_event_t *humidity, Isensors_event_t *temp) override;
 
 private:
     Adafruit_SHT4x sht4x; // The concrete SHT4x instance
     Iht_precision_t currentPrecision;
 
     // Helper to map between interface and implementation enums
     sht4x_precision_t mapPrecision(Iht_precision_t prec);
     Iht_precision_t mapPrecision(sht4x_precision_t prec);
 };
 
 #endif // HUMIDITY_TEMPERATURE_ADAFRUIT_SHT4X_H