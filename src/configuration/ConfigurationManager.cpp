/**
 * @file ConfigurationManager.cpp
 * @brief Implementation of ConfigurationManager class
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #include "ConfigurationManager.h"

 ConfigurationManager::ConfigurationManager() {};
 

 bool ConfigurationManager::setConfiguration(std::string config) {
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
     config += "}}";
     
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
             m_numAuxTalons = extractJsonIntField(systemJson, "numAuxTalons", 1);
             m_numI2CTalons = extractJsonIntField(systemJson, "numI2CTalons", 1);
             m_numSDI12Talons = extractJsonIntField(systemJson, "numSDI12Talons", 1);
             
             updateSystemConfigurationUid();
         }
     }
     // Find the sensor configuration section
     size_t sensorsStart = configStr.find("\"sensors\":{");
     if (sensorsStart != std::string::npos) {
         size_t sensorsEnd = findMatchingBracket(configStr, sensorsStart + 10);
         if (sensorsEnd > 0) {
             std::string sensorsJson = configStr.substr(sensorsStart + 10, sensorsEnd - (sensorsStart + 10));
            
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