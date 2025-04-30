/**
 * @file MockAdafruit_SHT4X.h
 * @brief Mock implementation of IHumidityTemperature for testing
 *
 * Provides a mock implementation of the IHumidityTemperature interface
 * that can be used for unit testing Kestrel without hardware dependencies.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef MOCK_ADAFRUIT_SHT4X_H
 #define MOCK_ADAFRUIT_SHT4X_H
 
 #include "IHumidityTemperature.h"
 #include <gmock/gmock.h>
 
 /**
  * @brief Mock implementation of IHumidityTemperature for testing
  */
 class MockAdafruit_SHT4X : public IHumidityTemperature {
 public:
     // Default constructor and destructor
     MockAdafruit_SHT4X() = default;
     ~MockAdafruit_SHT4X() override = default;
 
     // Mock methods
     MOCK_METHOD(bool, begin, (), (override));
     MOCK_METHOD(void, setPrecision, (ht_precision_t prec), (override));
     MOCK_METHOD(ht_precision_t, getPrecision, (), (override));
     MOCK_METHOD(bool, getEvent, (sensors_event_t *humidity, sensors_event_t *temp), (override));
 };
 
 #endif // MOCK_ADAFRUIT_SHT4X_H