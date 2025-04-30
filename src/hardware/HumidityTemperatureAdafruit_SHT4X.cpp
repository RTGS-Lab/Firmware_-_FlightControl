/**
 * @file HumidityTemperatureAdafruit_SHT4X.cpp
 * @brief Implementation of HumidityTemperatureAdafruit_SHT4X class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "HumidityTemperatureAdafruit_SHT4X.h"

 HumidityTemperatureAdafruit_SHT4X::HumidityTemperatureAdafruit_SHT4X() : currentPrecision(HT_HIGH_PRECISION) {
     // Constructor initializes with high precision by default
 }
 
 bool HumidityTemperatureAdafruit_SHT4X::begin() {
     // Initialize the underlying Adafruit_SHT4x sensor
     return sht4x.begin();
 }
 
 void HumidityTemperatureAdafruit_SHT4X::setPrecision(Iht_precision_t prec) {
     // Store current precision setting
     currentPrecision = prec;
     
     // Convert to Adafruit enum and apply to the hardware
     sht4x_precision_t adafruitPrec = mapPrecision(prec);
     sht4x.setPrecision(adafruitPrec);
 }
 
 Iht_precision_t HumidityTemperatureAdafruit_SHT4X::getPrecision() {
     // Return the stored precision setting
     return currentPrecision;
 }
 
 bool HumidityTemperatureAdafruit_SHT4X::getEvent(Isensors_event_t *humidity, Isensors_event_t *temp) {
    //translate Isensors_event_t to Adafruit_Sensor event
     sensors_event_t humidityEvent;
     sensors_event_t tempEvent;
     
     // Call the underlying implementation to get the events
     bool success = sht4x.getEvent(&humidityEvent, &tempEvent);
     
     //set the interface struct values
     humidity->relative_humidity = humidityEvent.relative_humidity;
     temp->temperature = tempEvent.temperature;

     return success;
 }
 
 sht4x_precision_t HumidityTemperatureAdafruit_SHT4X::mapPrecision(Iht_precision_t prec) {
     // Map interface precision enum to implementation precision enum
     switch (prec) {
         case HT_HIGH_PRECISION:
             return SHT4X_HIGH_PRECISION;
         case HT_MED_PRECISION:
             return SHT4X_MED_PRECISION;
         case HT_LOW_PRECISION:
             return SHT4X_LOW_PRECISION;
         default:
             return SHT4X_HIGH_PRECISION; // Default to high precision
     }
 }
 
 Iht_precision_t HumidityTemperatureAdafruit_SHT4X::mapPrecision(sht4x_precision_t prec) {
     // Map implementation precision enum to interface precision enum
     switch (prec) {
         case SHT4X_HIGH_PRECISION:
             return HT_HIGH_PRECISION;
         case SHT4X_MED_PRECISION:
             return HT_MED_PRECISION;
         case SHT4X_LOW_PRECISION:
             return HT_LOW_PRECISION;
         default:
             return HT_HIGH_PRECISION; // Default to high precision
     }
 }