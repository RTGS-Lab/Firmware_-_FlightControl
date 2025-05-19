// SensorManager.cpp - Implementation
#include "SensorManager.h"
#include "ConfigurationManager.h"
#include <AuxTalon.h>
#include <I2CTalon.h>
#include <SDI12Talon.h>
#include <Haar.h>
#include <SO421.h>
#include <SP421.h>
#include <TDR315H.h>
#include <Hedorah.h>
#include <T9602.h>
#include <Li710.h>
#include <BaroVue10.h>

SensorManager::SensorManager(ConfigurationManager& configManager) 
    : configManager(configManager) {
}

void SensorManager::initializeSensors(ITimeProvider& timeProvider, ISDI12Talon& sdi12Interface) {
    // Clear existing sensors
    clearAllSensors();
    
    // Create Talons
    for (int i = 0; i < configManager.getNumAuxTalons(); i++) {
        auxTalons.push_back(ConfigurationManager::createAuxTalon());
    }
    
    for (int i = 0; i < configManager.getNumI2CTalons(); i++) {
        i2cTalons.push_back(ConfigurationManager::createI2CTalon());
    }
    
    for (int i = 0; i < configManager.getNumSDI12Talons(); i++) {
        sdi12Talons.push_back(ConfigurationManager::createSDI12Talon());
    }
    
    // Create Sensors
    for (int i = 0; i < configManager.getNumHaar(); i++) {
        haarSensors.push_back(ConfigurationManager::createHaarSensor());
    }
    
    // For SDI12-based sensors, use the first available SDI12 talon
    if (!sdi12Talons.empty()) {
        SDI12Talon* firstSDI12Talon = sdi12Talons[0].get();
        
        for (int i = 0; i < configManager.getNumO2(); i++) {
            apogeeO2Sensors.push_back(ConfigurationManager::createO2Sensor(*firstSDI12Talon));
        }
        
        for (int i = 0; i < configManager.getNumApogeeSolar(); i++) {
            apogeeSolarSensors.push_back(ConfigurationManager::createSolarSensor(*firstSDI12Talon));
        }
        
        for (int i = 0; i < configManager.getNumSoil(); i++) {
            soilSensors.push_back(ConfigurationManager::createSoilSensor(*firstSDI12Talon));
        }
        
        for (int i = 0; i < configManager.getNumET(); i++) {
            etSensors.push_back(ConfigurationManager::createETSensor(timeProvider, sdi12Interface));
        }
        
        for (int i = 0; i < configManager.getNumPressure(); i++) {
            pressureSensors.push_back(ConfigurationManager::createPressureSensor(*firstSDI12Talon));
        }
    }
    
    for (int i = 0; i < configManager.getNumCO2(); i++) {
        gasSensors.push_back(ConfigurationManager::createCO2Sensor());
    }
}

void SensorManager::clearAllSensors() {
    auxTalons.clear();
    i2cTalons.clear();
    sdi12Talons.clear();
    haarSensors.clear();
    apogeeO2Sensors.clear();
    apogeeSolarSensors.clear();
    soilSensors.clear();
    gasSensors.clear();
    humiditySensors.clear();
    etSensors.clear();
    pressureSensors.clear();
}

std::vector<Talon*> SensorManager::getAllTalons() {
    std::vector<Talon*> allTalons;
    
    for (auto& talon : auxTalons) {
        allTalons.push_back(talon.get());
    }
    
    for (auto& talon : i2cTalons) {
        allTalons.push_back(talon.get());
    }
    
    for (auto& talon : sdi12Talons) {
        allTalons.push_back(talon.get());
    }
    
    return allTalons;
}

std::vector<Sensor*> SensorManager::getAllSensors() {
    std::vector<Sensor*> allSensors;
    
    // Add all sensor types to the vector
    for (auto& sensor : haarSensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : apogeeO2Sensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : apogeeSolarSensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : soilSensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : gasSensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : humiditySensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : etSensors) {
        allSensors.push_back(sensor.get());
    }
    
    for (auto& sensor : pressureSensors) {
        allSensors.push_back(sensor.get());
    }
    
    return allSensors;
}

int SensorManager::getTotalSensorCount() const {
    // Core sensors (3) + Talons + Other Sensors
    return 3 + auxTalons.size() + i2cTalons.size() + sdi12Talons.size() +
           haarSensors.size() + apogeeO2Sensors.size() + apogeeSolarSensors.size() +
           soilSensors.size() + gasSensors.size() + humiditySensors.size() +
           etSensors.size() + pressureSensors.size();
}

// ConfigurationManager.cpp - Updated factory methods
std::unique_ptr<AuxTalon> ConfigurationManager::createAuxTalon() {
    return std::make_unique<AuxTalon>(0, 0x14); // Default port and hardware version
}

std::unique_ptr<I2CTalon> ConfigurationManager::createI2CTalon() {
    return std::make_unique<I2CTalon>(0, 0x21); // Default port and hardware version
}

std::unique_ptr<SDI12Talon> ConfigurationManager::createSDI12Talon() {
    return std::make_unique<SDI12Talon>(0, 0x14); // Default port and hardware version
}

std::unique_ptr<Haar> ConfigurationManager::createHaarSensor() {
    return std::make_unique<Haar>(0, 0, 0x20); // Default ports and version
}

std::unique_ptr<SO421> ConfigurationManager::createO2Sensor(SDI12Talon& talon) {
    return std::make_unique<SO421>(talon, 0, 0); // Default ports and version
}

std::unique_ptr<SP421> ConfigurationManager::createSolarSensor(SDI12Talon& talon) {
    return std::make_unique<SP421>(talon, 0, 0); // Default ports and version
}

std::unique_ptr<TDR315H> ConfigurationManager::createSoilSensor(SDI12Talon& talon) {
    return std::make_unique<TDR315H>(talon, 0, 0); // Default ports and version
}

std::unique_ptr<Hedorah> ConfigurationManager::createCO2Sensor() {
    return std::make_unique<Hedorah>(0, 0, 0x10); // Default ports and version
}

std::unique_ptr<T9602> ConfigurationManager::createHumiditySensor() {
    return std::make_unique<T9602>(0, 0, 0x00); // Default ports and version
}

std::unique_ptr<LI710> ConfigurationManager::createETSensor(ITimeProvider& timeProvider, ISDI12Talon& talon) {
    return std::make_unique<LI710>(timeProvider, talon, 0, 0); // Default ports and version
}

std::unique_ptr<BaroVue10> ConfigurationManager::createPressureSensor(SDI12Talon& talon) {
    return std::make_unique<BaroVue10>(talon, 0, 0x00); // Default ports and version
}