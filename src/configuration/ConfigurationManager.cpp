/**
 * @file ConfigurationManager.cpp
 * @brief Implementation of ConfigurationManager class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "ConfigurationManager.h"
 #include <Particle.h>
 #include <Kestrel.h>  // Include for numTalonPorts
 
 // External globals needed for configuration
 extern Talon* talons[];
 extern ITimeProvider& realTimeProvider;
 extern SDI12Talon& sdi12;
 
 ConfigurationManager::ConfigurationManager()
     : m_sensorCount(0)
     , m_availableSensors(nullptr)
     , m_availableSensorCount(0)
     , m_logPeriod(300)
     , m_backhaulCount(4)
     , m_powerSaveMode(1)
     , m_loggingMode(0)
     // Initialize sensor objects with proper construction parameters
     , m_haar(0, 0, 0x00)
     , m_hedorah(0, 0, 0x00)
     , m_t9602(0, 0, 0x00)
     , m_tdr315h{sdi12, 0, 0, 0x00, sdi12, 0, 0, 0x00, sdi12, 0, 0, 0x00} // Initialize 3 TDR315H sensors
     , m_teros11{sdi12, 0, 0, 0x00, sdi12, 0, 0, 0x00} // Initialize 2 TEROS11 sensors
     , m_li710(realTimeProvider, sdi12, 0, 0)
     , m_so421(sdi12, 0, 0)
     , m_sp421(sdi12, 0, 0)
     , m_atmos22(sdi12, 0, 0)
     , m_barovue(sdi12, 0, 0x00)
 {
     // Initialize all sensors as not in use and not enabled
     for (int i = 0; i < MAX_TOTAL_SENSORS; i++) {
         m_sensorEnabled[i] = false;
     }
     
     // Initialize dynamic sensors as not in use
     for (int i = 0; i < MAX_DYNAMIC_SENSORS; i++) {
         m_dynamicSensorInUse[i] = false;
     }
 }
 
 void ConfigurationManager::registerAvailableSensors(Sensor** availableSensors, uint8_t count) {
     m_availableSensors = availableSensors;
     m_availableSensorCount = count;
     
     // Clear the sensor pool
     clearSensorPool();
 }
 
 bool ConfigurationManager::setConfiguration(std::string config) {
     // Fully reset all sensors
     resetSensors();
     
     // Parse and apply the configuration
     return parseConfiguration(config);
 }
 
 std::string ConfigurationManager::getConfiguration() {
     std::string config = "{\"config\":{";
     
     // System configuration
     config += "\"system\":{";
     config += "\"logPeriod\":" + std::to_string(m_logPeriod) + ",";
     config += "\"backhaulCount\":" + std::to_string(m_backhaulCount) + ",";
     config += "\"powerSaveMode\":" + std::to_string(m_powerSaveMode) + ",";
     config += "\"loggingMode\":" + std::to_string(m_loggingMode);
     config += "},";
     
     // Sensors configuration
     config += "\"sensors\":[";
     
     // Helper function to determine sensor type based on pointer comparison
     auto getSensorType = [this](Sensor* sensor) -> std::string {
         // Check core sensors first based on the order in available sensors
         if (m_availableSensorCount > 0 && sensor == m_availableSensors[0]) return "FileSystem";
         if (m_availableSensorCount > 1 && sensor == m_availableSensors[1]) return "Aux";
         if (m_availableSensorCount > 2 && sensor == m_availableSensors[2]) return "I2C";
         if (m_availableSensorCount > 3 && sensor == m_availableSensors[3]) return "SDI12";
         if (m_availableSensorCount > 4 && sensor == m_availableSensors[4]) return "Battery";
         if (m_availableSensorCount > 5 && sensor == m_availableSensors[5]) return "Logger";
         
         // Check dynamic sensors
         if (sensor == &m_haar) return "Haar";
         if (sensor == &m_hedorah) return "Hedorah";
         if (sensor == &m_t9602) return "T9602";
         for (int i = 0; i < 3; i++) if (sensor == &m_tdr315h[i]) return "TDR315H";
         for (int i = 0; i < 2; i++) if (sensor == &m_teros11[i]) return "TEROS11";
         if (sensor == &m_li710) return "LI710";
         if (sensor == &m_so421) return "SO421";
         if (sensor == &m_sp421) return "SP421";
         if (sensor == &m_atmos22) return "ATMOS22";
         if (sensor == &m_barovue) return "BaroVue10";
         
         return "Unknown"; // Fallback
     };
     
     bool firstSensor = true;
     // Add all enabled sensors to the configuration
     for (uint8_t i = 0; i < m_sensorCount; i++) {
         if (!firstSensor) config += ",";
         firstSensor = false;
         
         config += "{";
         config += "\"type\":\"" + getSensorType(m_sensorPool[i]) + "\",";
         config += "\"enabled\":true,";
         
         // Only include port and talonPort if they're defined (non-zero)
         if (m_sensorPool[i]->getSensorPort() > 0) {
             config += "\"port\":" + std::to_string(m_sensorPool[i]->getSensorPort()) + ",";
         }
         
         if (m_sensorPool[i]->getTalonPort() > 0) {
             config += "\"talonPort\":" + std::to_string(m_sensorPool[i]->getTalonPort()) + ",";
         }
         
         // We can't determine version anymore without getVersion,
         // so we'll remove this part
         
         // Remove trailing comma if present
         if (config.back() == ',') {
             config.pop_back();
         }
         
         config += "}";
     }
     
     // Also add disabled core sensors with enabled:false
     for (uint8_t i = 0; i < m_availableSensorCount; i++) {
         bool isEnabled = false;
         for (uint8_t j = 0; j < m_sensorCount; j++) {
             if (m_sensorPool[j] == m_availableSensors[i]) {
                 isEnabled = true;
                 break;
             }
         }
         
         if (!isEnabled) {
             if (!firstSensor) config += ",";
             firstSensor = false;
             
             config += "{";
             
             // Get type based on index in availableSensors
             std::string type;
             switch (i) {
                 case 0: type = "FileSystem"; break;
                 case 1: type = "Aux"; break;
                 case 2: type = "I2C"; break;
                 case 3: type = "SDI12"; break;
                 case 4: type = "Battery"; break;
                 case 5: type = "Logger"; break;
                 default: type = "Unknown"; break;
             }
             
             config += "\"type\":\"" + type + "\",";
             config += "\"enabled\":false";
             config += "}";
         }
     }
     
     config += "]";
     config += "}}";
     
     return config;
 }
 
 Sensor** ConfigurationManager::getSensorArray() {
     return m_sensorPool;
 }
 
 uint8_t ConfigurationManager::getSensorCount() const {
     return m_sensorCount;
 }
 
 void ConfigurationManager::clearSensorPool() {
     // Reset to empty pool
     m_sensorCount = 0;
     
     // Mark all pre-allocated sensors as not in use
     for (int i = 0; i < MAX_DYNAMIC_SENSORS; i++) {
         m_dynamicSensorInUse[i] = false;
     }
 }
 
 void ConfigurationManager::resetSensors() {
     // Clear the sensor pool
     clearSensorPool();
     
     // Reset all pre-allocated sensors (completely safe as all these objects are statically allocated)
     m_haar.setTalonPort(0);
     m_haar.setSensorPort(0);
     
     m_hedorah.setTalonPort(0);
     m_hedorah.setSensorPort(0);
     
     m_t9602.setTalonPort(0);
     m_t9602.setSensorPort(0);
     
     // Reset TDR315H sensors
     for (int i = 0; i < 3; i++) {
         m_tdr315h[i].setTalonPort(0);
         m_tdr315h[i].setSensorPort(0);
     }
     
     // Reset TEROS11 sensors
     for (int i = 0; i < 2; i++) {
         m_teros11[i].setTalonPort(0);
         m_teros11[i].setSensorPort(0);
     }
     
     m_li710.setTalonPort(0);
     m_li710.setSensorPort(0);
     
     m_so421.setTalonPort(0);
     m_so421.setSensorPort(0);
     
     m_sp421.setTalonPort(0);
     m_sp421.setSensorPort(0);
     
     m_atmos22.setTalonPort(0);
     m_atmos22.setSensorPort(0);
     
     m_barovue.setTalonPort(0);
     m_barovue.setSensorPort(0);
 }
 
 bool ConfigurationManager::parseConfiguration(const std::string& configStr) {
     // Find the system configuration section
     size_t systemStart = configStr.find("\"system\":{");
     if (systemStart != std::string::npos) {
         size_t systemEnd = findMatchingBracket(configStr, systemStart + 9);
         if (systemEnd > 0) {
             std::string systemJson = configStr.substr(systemStart + 9, systemEnd - (systemStart + 9));
             
             // Parse system settings
             m_logPeriod = extractJsonIntField(systemJson, "logPeriod", 300);
             m_backhaulCount = extractJsonIntField(systemJson, "backhaulCount", 4);
             m_powerSaveMode = extractJsonIntField(systemJson, "powerSaveMode", 1);
             m_loggingMode = extractJsonIntField(systemJson, "loggingMode", 0);
         }
     }
     
     // Find the sensors array
     size_t sensorsStart = configStr.find("\"sensors\":[");
     if (sensorsStart == std::string::npos) {
         return false;
     }
     
     size_t sensorsEnd = findMatchingBracket(configStr, sensorsStart + 10);
     if (sensorsEnd == std::string::npos) {
         return false;
     }
     
     // Extract sensors array content
     std::string sensorsJson = configStr.substr(sensorsStart + 10, sensorsEnd - (sensorsStart + 10));
     
     // Count sensors to check if we exceed the limit
     int sensorCount = 0;
     size_t pos = 0;
     while ((pos = sensorsJson.find('{', pos)) != std::string::npos) {
         sensorCount++;
         pos++;
     }
     
     // Check if we have too many sensors
     if (sensorCount > MAX_TOTAL_SENSORS) {
         Serial.printlnf("Error: Too many sensors in configuration (%d > %d)", 
                         sensorCount, MAX_TOTAL_SENSORS);
         return false;
     }
     
     // Parse each sensor object
     pos = 0;
     while (pos < sensorsJson.length()) {
         size_t objStart = sensorsJson.find('{', pos);
         if (objStart == std::string::npos) break;
         
         size_t objEnd = findMatchingBracket(sensorsJson, objStart);
         if (objEnd == std::string::npos) break;
         
         std::string sensorObj = sensorsJson.substr(objStart + 1, objEnd - objStart - 1);
         
         // Extract sensor properties
         std::string type = extractJsonField(sensorObj, "type");
         if (type.empty()) {
             Serial.println("Error: Sensor type missing");
             pos = objEnd + 1;
             continue;
         }
         
         // Check if this sensor is enabled
         bool enabled = extractJsonBoolField(sensorObj, "enabled", true);
         
         if (enabled) {
             // Check if this is a core sensor first
             if (!enableCoreSensor(type)) {
                 // If not a core sensor, try to configure a dynamic sensor
                 int port = extractJsonIntField(sensorObj, "port", 0);
                 int talonPort = extractJsonIntField(sensorObj, "talonPort", 0);
                 std::string versionStr = extractJsonField(sensorObj, "version");
                 
                 int version = 0;
                 if (versionStr.substr(0, 2) == "0x") {
                     version = strtol(versionStr.c_str() + 2, NULL, 16);
                 } else if (!versionStr.empty()) {
                     version = atoi(versionStr.c_str());
                 }
                 
                 if (!configureSensor(type, port, talonPort, version)) {
                     Serial.printlnf("Warning: Failed to configure sensor %s", type.c_str());
                 }
             }
         }
         
         pos = objEnd + 1;
     }
     
     return true;
 }
 
 Sensor* ConfigurationManager::findAvailableSensorByType(const std::string& type) {
     // Map sensor types to indices in the available sensors array
     // This is a workaround since the Sensor class doesn't have a getType() method
     
     // Core sensor types mapping to indices in availableSensors
     if (type == "FileSystem" && m_availableSensorCount > 0) return m_availableSensors[0];
     if (type == "Aux" && m_availableSensorCount > 1) return m_availableSensors[1];
     if (type == "I2C" && m_availableSensorCount > 2) return m_availableSensors[2];
     if (type == "SDI12" && m_availableSensorCount > 3) return m_availableSensors[3];
     if (type == "Battery" && m_availableSensorCount > 4) return m_availableSensors[4];
     if (type == "Logger" && m_availableSensorCount > 5) return m_availableSensors[5];
     
     return nullptr; // Type not found
 }
 
 bool ConfigurationManager::enableCoreSensor(const std::string& type) {
     // Find a sensor with matching type in available sensors
     Sensor* sensor = findAvailableSensorByType(type);
     if (sensor) {
         // Check if it's already in the pool
         for (uint8_t i = 0; i < m_sensorCount; i++) {
             if (m_sensorPool[i] == sensor) {
                 // Already enabled
                 return true;
             }
         }
         
         // Add to pool if not already there
         if (m_sensorCount < MAX_TOTAL_SENSORS) {
             m_sensorPool[m_sensorCount++] = sensor;
             return true;
         }
     }
     return false;
 }
 
 bool ConfigurationManager::configureSensor(const std::string& type, int port, int talonPort, int version) {
     // If we're already at capacity, fail
     if (m_sensorCount >= MAX_TOTAL_SENSORS) {
         return false;
     }
     
     // Find appropriate Talon
     Talon* talon = nullptr;
     if (talonPort > 0 && talonPort <= Kestrel::numTalonPorts) {
         talon = talons[talonPort - 1];
     }
     
     // Configure the appropriate pre-allocated sensor
     if (type == "Haar") {
         if (!m_dynamicSensorInUse[0]) {
             m_haar.setTalonPort(talonPort);
             m_haar.setSensorPort(port);
             // Version is set during sensor constructor, not supported via setVersion
             m_sensorPool[m_sensorCount++] = &m_haar;
             m_dynamicSensorInUse[0] = true;
             return true;
         }
     }
     else if (type == "Hedorah") {
         if (!m_dynamicSensorInUse[1]) {
             m_hedorah.setTalonPort(talonPort);
             m_hedorah.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_hedorah;
             m_dynamicSensorInUse[1] = true;
             return true;
         }
     }
     else if (type == "T9602") {
         if (!m_dynamicSensorInUse[2]) {
             m_t9602.setTalonPort(talonPort);
             m_t9602.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_t9602;
             m_dynamicSensorInUse[2] = true;
             return true;
         }
     }
     else if (type == "TDR315H") {
         // Find an available TDR315H instance
         for (int i = 0; i < 3; i++) {
             if (!m_dynamicSensorInUse[3 + i]) {
                 m_tdr315h[i].setTalonPort(talonPort);
                 m_tdr315h[i].setSensorPort(port);
                 // Version is set during sensor constructor
                 m_sensorPool[m_sensorCount++] = &m_tdr315h[i];
                 m_dynamicSensorInUse[3 + i] = true;
                 return true;
             }
         }
     }
     else if (type == "TEROS11") {
         // Find an available TEROS11 instance
         for (int i = 0; i < 2; i++) {
             if (!m_dynamicSensorInUse[6 + i]) {
                 m_teros11[i].setTalonPort(talonPort);
                 m_teros11[i].setSensorPort(port);
                 // Version is set during sensor constructor
                 m_sensorPool[m_sensorCount++] = &m_teros11[i];
                 m_dynamicSensorInUse[6 + i] = true;
                 return true;
             }
         }
     }
     else if (type == "LI710") {
         if (!m_dynamicSensorInUse[8]) {
             m_li710.setTalonPort(talonPort);
             m_li710.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_li710;
             m_dynamicSensorInUse[8] = true;
             return true;
         }
     }
     else if (type == "SO421") {
         if (!m_dynamicSensorInUse[9]) {
             m_so421.setTalonPort(talonPort);
             m_so421.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_so421;
             m_dynamicSensorInUse[9] = true;
             return true;
         }
     }
     else if (type == "SP421") {
         if (!m_dynamicSensorInUse[10]) {
             m_sp421.setTalonPort(talonPort);
             m_sp421.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_sp421;
             m_dynamicSensorInUse[10] = true;
             return true;
         }
     }
     else if (type == "ATMOS22") {
         if (!m_dynamicSensorInUse[11]) {
             m_atmos22.setTalonPort(talonPort);
             m_atmos22.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_atmos22;
             m_dynamicSensorInUse[11] = true;
             return true;
         }
     }
     else if (type == "BaroVue10") {
         if (!m_dynamicSensorInUse[12]) {
             m_barovue.setTalonPort(talonPort);
             m_barovue.setSensorPort(port);
             // Version is set during sensor constructor
             m_sensorPool[m_sensorCount++] = &m_barovue;
             m_dynamicSensorInUse[12] = true;
             return true;
         }
     }
     // Add more sensor types as needed
     
     return false; // Failed to configure sensor
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