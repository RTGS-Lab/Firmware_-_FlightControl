/**
 * @file AccelerometerMXC6655.h
 * @brief Concrete implementation of IAccelerometer using MXC6655
 *
 * Adapts the MXC6655 accelerometer to the IAccelerometer interface
 * for dependency injection and testing.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef ACCELEROMETER_MXC6655_H
 #define ACCELEROMETER_MXC6655_H
 
 #include "IAccelerometer.h"
 #include "MXC6655.h"
 
 /**
  * @brief Concrete implementation of IAccelerometer using MXC6655
  */
 class AccelerometerMXC6655 : public IAccelerometer {
 public:
     /**
      * @brief Constructor
      */
     AccelerometerMXC6655();
     ~AccelerometerMXC6655() override = default;
 
     int begin() override;
     float getAccel(uint8_t axis, uint8_t range = 0) override;
     int updateAccelAll() override;
     float getTemp() override;
     float* getData() override;
     float* getOffset() override;
     void setOffset(float offsetX, float offsetY, float offsetZ) override;
 
 private:
     MXC6655 accel; // The concrete MXC6655 instance
 };
 
 #endif // ACCELEROMETER_MXC6655_H