/**
 * @file ConfigurationManager.cpp
 * @brief Implementation of the ConfigurationManager class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#include "ConfigurationManager.h"
#include <fstream>
#include <iostream>

// For Particle-based systems
#ifndef TESTING
#include "Particle.h"
#else
#include "test/mocks/Particle.h"
#endif

ConfigurationManager::ConfigurationManager() : m_currentConfig("") {
    // Constructor implementation
}

ConfigurationManager::~ConfigurationManager() {
    // Destructor implementation
}

bool ConfigurationManager::begin(bool defaultConfig) {
    if (defaultConfig) {
        return loadDefaultConfiguration();
    }
    return true;
}

bool ConfigurationManager::setConfiguration(std::string config) {
    // Validate the configuration format
    // Here you would validate the JSON structure
    
    // Store the configuration
    m_currentConfig = config;
    
    // Apply the configuration
    return applyConfiguration();
}

std::string ConfigurationManager::getConfiguration() {
    return m_currentConfig;
}

bool ConfigurationManager::loadDefaultConfiguration() {
    // Load the default configuration from the DefaultConfiguration.json file
    std::ifstream configFile("configuration/DefaultConfiguration.json");
    if (configFile.is_open()) {
        std::string config((std::istreambuf_iterator<char>(configFile)),
                           std::istreambuf_iterator<char>());
        configFile.close();
        
        m_currentConfig = config;
        return true;
    }
    
    // If file can't be opened, use a hardcoded minimal configuration
    m_currentConfig = R"({
  "config": {
    "system": {
      "backhaul_context": "4",
      "log_period": "300",
      "power_save_mode": "1",
      "logging_mode": "0"
    },
    "peripherals": {},
    "sensors": {}
  }
})";
    
    return false;
}

bool ConfigurationManager::saveConfiguration() {
    // Save the current configuration to persistent storage
    // This could be a file on an SD card, EEPROM, etc.
    
    // Example: Save to a file
    std::ofstream configFile("configuration/CurrentConfiguration.json");
    if (configFile.is_open()) {
        configFile << m_currentConfig;
        configFile.close();
        return true;
    }
    
    return false;
}

bool ConfigurationManager::applyConfiguration() {
    // Parse the configuration (assuming JSON format)
    // Apply settings to the appropriate components
    
    // Example: Log that configuration is being applied
    Serial.println("Applying configuration...");
    
    // TODO: Implementation would include:
    // 1. Parse JSON to extract system settings
    // 2. Configure system parameters like log period, power save mode, etc.
    // 3. Enable/disable peripherals as specified
    // 4. Configure and initialize sensors based on their settings
    
    // Example pseudocode:
    // JsonDocument doc;
    // DeserializationError error = deserializeJson(doc, m_currentConfig);
    // 
    // if (error) {
    //     Serial.println("Failed to parse configuration");
    //     return false;
    // }
    // 
    // // Apply system settings
    // const char* logPeriod = doc["config"]["system"]["log_period"];
    // const char* powerSaveMode = doc["config"]["system"]["power_save_mode"];
    // const char* loggingMode = doc["config"]["system"]["logging_mode"];
    // 
    // // Configure system with these settings
    // setLogPeriod(atoi(logPeriod));
    // setPowerSaveMode(atoi(powerSaveMode));
    // setLoggingMode(atoi(loggingMode));
    // 
    // // Enable/configure peripherals
    // JsonObject peripherals = doc["config"]["peripherals"];
    // for (JsonPair peripheral : peripherals) {
    //     const char* id = peripheral.value()["id"];
    //     bool enabled = peripheral.value()["enabled"];
    //     
    //     // Configure the peripheral
    //     configurePheripheral(id, enabled);
    // }
    // 
    // // Configure sensors
    // JsonObject sensors = doc["config"]["sensors"];
    // for (JsonPair sensor : sensors) {
    //     const char* id = sensor.value()["id"];
    //     bool enabled = sensor.value()["enabled"];
    //     int port = sensor.value()["port"] | 0;  // Default to 0 if not specified
    //     
    //     // Configure the sensor
    //     configureSensor(id, enabled, port);
    // }
    
    return true;
}