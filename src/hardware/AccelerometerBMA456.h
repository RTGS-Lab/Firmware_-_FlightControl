/**
 * @file AccelerometerBMA456.h
 * @brief Concrete implementation of IAccelerometer using BMA456
 *
 * Adapts the BMA456 accelerometer to the IAccelerometer interface
 * for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef ACCELEROMETER_BMA456_H
 #define ACCELEROMETER_BMA456_H
 
 #include "IAccelerometer.h"
 #include "arduino_bma456.h"
 
 /**
  * @brief Concrete implementation of IAccelerometer using BMA456
  */
 class AccelerometerBMA456 : public IAccelerometer {
 public:
     /**
      * @brief Constructor
      */
     AccelerometerBMA456();
     ~AccelerometerBMA456() override = default;
 
     int begin() override;
     float getAccel(uint8_t axis, uint8_t range = 0) override;
     int updateAccelAll() override;
     float getTemp() override;
     float* getData() override;
     float* getOffset() override;
     void setOffset(float offsetX, float offsetY, float offsetZ) override;
 
 private:
     BMA456 accel; // The concrete BMA456 instance
 };
 
 #endif // ACCELEROMETER_BMA456_H