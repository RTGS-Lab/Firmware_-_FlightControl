// SensorManager.cpp - Implementation
#include "SensorManager.h"
#include "ConfigurationManager.h"

SensorManager::SensorManager(ConfigurationManager& configManager) 
    : configManager(configManager) {
}

void SensorManager::initializeSensors(ITimeProvider& timeProvider, ISDI12Talon& sdi12Interface) {
    // Clear existing sensors
    clearAllSensors();
    
    // Initialize talons first
    initializeTalons();
    
    // Initialize sensors
    initializeSensorsOnly(timeProvider, sdi12Interface);
}

void SensorManager::initializeTalons() {
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
}

void SensorManager::initializeSensorsOnly(ITimeProvider& timeProvider, ISDI12Talon& sdi12Interface) {
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