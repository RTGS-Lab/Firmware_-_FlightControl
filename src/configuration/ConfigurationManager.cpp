/**
 * @file ConfigurationManager.cpp
 * @brief Implementation of ConfigurationManager class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "ConfigurationManager.h"
 #include <algorithm>
 #include <cctype>
 
 #ifndef TESTING
 #include "Particle.h"
 #else
 #include "MockParticle.h"
 #endif

 // Static member definitions
 const int ConfigurationManager::EEPROM_CONFIG_START;
 const int ConfigurationManager::EEPROM_SYSTEM_UID_ADDR;
 const int ConfigurationManager::EEPROM_SENSOR_UID_ADDR;
 const int ConfigurationManager::EEPROM_CONFIG_VALID_FLAG;
 const uint8_t ConfigurationManager::EEPROM_VALID_MARKER;

 ConfigurationManager::ConfigurationManager() {};
 

 bool ConfigurationManager::setConfiguration(std::string config) {
     // Parse and apply the configuration
     bool success = parseConfiguration(config);
     if (success) {
         // Save to EEPROM as backup
         saveConfigToEEPROM();
     }
     return success;
 }
 
 std::string ConfigurationManager::getConfiguration() {
     std::string config = "{\"config\":{";
     
     // System configuration
     config += "\"system\":{";
     config += "\"logPeriod\":" + std::to_string(m_logPeriod) + ",";
     config += "\"backhaulCount\":" + std::to_string(m_backhaulCount) + ",";
     config += "\"powerSaveMode\":" + std::to_string(m_powerSaveMode) + ",";
     config += "\"loggingMode\":" + std::to_string(m_loggingMode) + ",";
     config += "\"numAuxTalons\":" + std::to_string(m_numAuxTalons) + ",";
     config += "\"numI2CTalons\":" + std::to_string(m_numI2CTalons) + ",";
     config += "\"numSDI12Talons\":" + std::to_string(m_numSDI12Talons);
     config += "},";
     
     // Sensor configuration
     config += "\"sensors\":{";
     config += "\"numET\":" + std::to_string(m_numET) + ",";
     config += "\"numHaar\":" + std::to_string(m_numHaar) + ",";
     config += "\"numSoil\":" + std::to_string(m_numSoil) + ",";
     config += "\"numApogeeSolar\":" + std::to_string(m_numApogeeSolar) + ",";
     config += "\"numCO2\":" + std::to_string(m_numCO2) + ",";
     config += "\"numO2\":" + std::to_string(m_numO2) + ",";
     config += "\"numPressure\":" + std::to_string(m_numPressure);
     config += "}}}";
     
     return config;
 }

 int ConfigurationManager::updateSystemConfigurationUid() {
    int tempUid = m_logPeriod << 16;
    tempUid |= m_backhaulCount << 12;
    tempUid |= m_powerSaveMode << 10;
    tempUid |= m_loggingMode << 8;
    tempUid |= m_numAuxTalons << 6;
    tempUid |= m_numI2CTalons << 4;
    tempUid |= m_numSDI12Talons << 2;
    m_SystemConfigUid = tempUid; 
    return m_SystemConfigUid;
 }

 int ConfigurationManager::updateSensorConfigurationUid() {
    int tempUid = m_numET << 28;
    tempUid |= m_numHaar << 24;
    tempUid |= m_numSoil << 20;
    tempUid |= m_numApogeeSolar << 16;
    tempUid |= m_numCO2 << 12;
    tempUid |= m_numO2 << 8;
    tempUid |= m_numPressure << 4;
    m_SensorConfigUid = tempUid; 
    return m_SensorConfigUid;
 }
 

 bool ConfigurationManager::parseConfiguration(const std::string& configStr) {
     // Remove all whitespace characters from the config string
     std::string cleanedConfig = configStr;
     cleanedConfig.erase(std::remove_if(cleanedConfig.begin(), cleanedConfig.end(),
                                        [](char c) { return std::isspace(c); }),
                         cleanedConfig.end());
     
     // Find the system configuration section
     size_t systemStart = cleanedConfig.find("\"system\":{");
     if (systemStart != std::string::npos) {
         size_t systemEnd = findMatchingBracket(cleanedConfig, systemStart + 9);
         if (systemEnd > 0) {
             std::string systemJson = cleanedConfig.substr(systemStart + 9, systemEnd - (systemStart + 9));
             
             // Parse system settings
             m_logPeriod = extractJsonIntField(systemJson, "logPeriod", 300);
             m_backhaulCount = extractJsonIntField(systemJson, "backhaulCount", 4);
             m_powerSaveMode = extractJsonIntField(systemJson, "powerSaveMode", 1);
             m_loggingMode = extractJsonIntField(systemJson, "loggingMode", 0);
             m_numAuxTalons = extractJsonIntField(systemJson, "numAuxTalons", 1);
             m_numI2CTalons = extractJsonIntField(systemJson, "numI2CTalons", 1);
             m_numSDI12Talons = extractJsonIntField(systemJson, "numSDI12Talons", 1);
             
             updateSystemConfigurationUid();
         }
     }
     // Find the sensor configuration section
     size_t sensorsStart = cleanedConfig.find("\"sensors\":{");
     if (sensorsStart != std::string::npos) {
         size_t sensorsEnd = findMatchingBracket(cleanedConfig, sensorsStart + 10);
         if (sensorsEnd > 0) {
             std::string sensorsJson = cleanedConfig.substr(sensorsStart + 10, sensorsEnd - (sensorsStart + 10));
            
             // Parse sensor settings
             m_numET = extractJsonIntField(sensorsJson, "numET", 0);
             m_numHaar = extractJsonIntField(sensorsJson, "numHaar", 0);
             m_numSoil = extractJsonIntField(sensorsJson, "numSoil", 3);
             m_numApogeeSolar = extractJsonIntField(sensorsJson, "numApogeeSolar", 0);
             m_numCO2 = extractJsonIntField(sensorsJson, "numCO2", 0);
             m_numO2 = extractJsonIntField(sensorsJson, "numO2", 0);
             m_numPressure = extractJsonIntField(sensorsJson, "numPressure", 0);
            
             updateSensorConfigurationUid();
         }
     }
     
     return true;
 }
 
 std::string ConfigurationManager::extractJsonField(const std::string& json, const std::string& fieldName) {
     std::string searchStr = "\"" + fieldName + "\":";
     size_t fieldStart = json.find(searchStr);
     if (fieldStart == std::string::npos) return "";
     
     fieldStart += searchStr.length();
     // Skip whitespace
     while (fieldStart < json.length() && isspace(json[fieldStart])) {
         fieldStart++;
     }
     
     // Check if string value
     if (json[fieldStart] == '"') {
         size_t valueEnd = json.find('"', fieldStart + 1);
         if (valueEnd == std::string::npos) return "";
         return json.substr(fieldStart + 1, valueEnd - fieldStart - 1);
     }
     
     // Must be numeric or boolean value
     size_t valueEnd = fieldStart;
     while (valueEnd < json.length() && 
           (isalnum(json[valueEnd]) || json[valueEnd] == '.' || json[valueEnd] == '-')) {
         valueEnd++;
     }
     
     return json.substr(fieldStart, valueEnd - fieldStart);
 }
 
 int ConfigurationManager::extractJsonIntField(const std::string& json, const std::string& fieldName, int defaultValue) {
     std::string value = extractJsonField(json, fieldName);
     if (value.empty()) return defaultValue;
     return atoi(value.c_str());
 }
 
 bool ConfigurationManager::extractJsonBoolField(const std::string& json, const std::string& fieldName, bool defaultValue) {
     std::string value = extractJsonField(json, fieldName);
     if (value.empty()) return defaultValue;
     return (value == "true" || value == "True" || value == "TRUE" || value == "1");
 }
 
 int ConfigurationManager::findMatchingBracket(const std::string& str, int openPos) {
     char open = str[openPos];
     char close;
     
     if (open == '{') close = '}';
     else if (open == '[') close = ']';
     else if (open == '(') close = ')';
     else return -1;
     
     int depth = 1;
     for (size_t i = openPos + 1; i < str.length(); i++) {
         if (str[i] == open) depth++;
         else if (str[i] == close) {
             depth--;
             if (depth == 0) return i;
         }
     }
     
     return -1; // No matching bracket found
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

// EEPROM backup functionality
bool ConfigurationManager::saveConfigToEEPROM() {
    // Write system and sensor UIDs (they encode all the config values)
    EEPROM.put(EEPROM_SYSTEM_UID_ADDR, m_SystemConfigUid);
    EEPROM.put(EEPROM_SENSOR_UID_ADDR, m_SensorConfigUid);
    
    // Write valid flag to indicate EEPROM contains valid config
    EEPROM.put(EEPROM_CONFIG_VALID_FLAG, EEPROM_VALID_MARKER);
    
    return true;
}

bool ConfigurationManager::loadConfigFromEEPROM() {
    // Check if EEPROM contains valid configuration
    uint8_t validFlag;
    EEPROM.get(EEPROM_CONFIG_VALID_FLAG, validFlag);
    if (validFlag != EEPROM_VALID_MARKER) {
        return false; // No valid config in EEPROM
    }
    
    // Read UIDs from EEPROM
    int systemUid, sensorUid;
    EEPROM.get(EEPROM_SYSTEM_UID_ADDR, systemUid);
    EEPROM.get(EEPROM_SENSOR_UID_ADDR, sensorUid);
    
    // Decode system UID back to configuration values (reverse of updateSystemConfigurationUid)
    m_logPeriod = (systemUid >> 16) & 0xFFFF;
    m_backhaulCount = (systemUid >> 12) & 0xF;
    m_powerSaveMode = (systemUid >> 10) & 0x3;
    m_loggingMode = (systemUid >> 8) & 0x3;
    m_numAuxTalons = (systemUid >> 6) & 0x3;
    m_numI2CTalons = (systemUid >> 4) & 0x3;
    m_numSDI12Talons = (systemUid >> 2) & 0x3;
    
    // Decode sensor UID back to configuration values (reverse of updateSensorConfigurationUid)
    m_numET = (sensorUid >> 28) & 0xF;
    m_numHaar = (sensorUid >> 24) & 0xF;
    m_numSoil = (sensorUid >> 20) & 0xF;
    m_numApogeeSolar = (sensorUid >> 16) & 0xF;
    m_numCO2 = (sensorUid >> 12) & 0xF;
    m_numO2 = (sensorUid >> 8) & 0xF;
    m_numPressure = (sensorUid >> 4) & 0xF;
    
    // Store the UIDs
    m_SystemConfigUid = systemUid;
    m_SensorConfigUid = sensorUid;
    
    return true;
}

void ConfigurationManager::clearConfigEEPROM() {
    // Clear the valid flag to invalidate EEPROM config
    EEPROM.put(EEPROM_CONFIG_VALID_FLAG, (uint8_t)0x00);
}