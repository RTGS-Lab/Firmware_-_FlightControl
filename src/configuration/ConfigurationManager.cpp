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