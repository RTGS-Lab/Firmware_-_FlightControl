/**
 * @file AccelerometerBMA456.cpp
 * @brief Implementation of the AccelerometerBMA456 class.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "AccelerometerBMA456.h"

 AccelerometerBMA456::AccelerometerBMA456() : accel() {
     // Default constructor, nothing to initialize here
 }
 
 int AccelerometerBMA456::begin() {
     return accel.begin();
 }
 
 float AccelerometerBMA456::getAccel(uint8_t axis, uint8_t range) {
    float x, y, z; 
    accel.getAcceleration(&x, &y, &z);
    switch(axis)
    {
        case 0:
        return x;
        case 1:
        return y;
        case 2:
        return z;
        default:
        return x;
    }
 }
 
 int AccelerometerBMA456::updateAccelAll() {
     accel.initialize();
     return 0;
 }
 
 float AccelerometerBMA456::getTemp() {
     return accel.getTemperature();
 }
 
 float* AccelerometerBMA456::getData() { //unimplemented
    return 0;
 }
 
 float* AccelerometerBMA456::getOffset() { //unimplemented
    return 0;
 }
 
 void AccelerometerBMA456::setOffset(float offsetX, float offsetY, float offsetZ) { //unimplemented
 }