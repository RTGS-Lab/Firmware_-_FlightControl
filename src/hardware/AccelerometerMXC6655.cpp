/**
 * @file AccelerometerMXC6655.cpp
 * @brief Implementation of the AccelerometerMXC6655 class.
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "AccelerometerMXC6655.h"

 AccelerometerMXC6655::AccelerometerMXC6655() : accel() {
     // Default constructor, nothing to initialize here
 }
 
 int AccelerometerMXC6655::begin() {
     return accel.begin();
 }
 
 float AccelerometerMXC6655::getAccel(uint8_t axis, uint8_t range) {
     return accel.getAccel(axis, range);
 }
 
 int AccelerometerMXC6655::updateAccelAll() {
     return accel.updateAccelAll();
 }
 
 float AccelerometerMXC6655::getTemp() {
     return accel.getTemp();
 }
 
 float* AccelerometerMXC6655::getData() {
     return accel.data;
 }
 
 float* AccelerometerMXC6655::getOffset() {
     return accel.offset;
 }
 
 void AccelerometerMXC6655::setOffset(float offsetX, float offsetY, float offsetZ) {
     accel.offset[0] = offsetX;
     accel.offset[1] = offsetY;
     accel.offset[2] = offsetZ;
 }