/**
 * @file ConfigurationManager.h
 * @brief Configuration manager for sensor deployment
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef CONFIGURATION_MANAGER_H
 #define CONFIGURATION_MANAGER_H
 
 #include "IConfiguration.h"
 class ConfigurationManager : public IConfiguration {
 public:
     ConfigurationManager();
     ~ConfigurationManager() = default; // No dynamic allocation, no need for custom destructor
     
     // IConfiguration implementation
     bool setConfiguration(std::string config) override;
     std::string getConfiguration() override;
     
     // System configuration getters
     unsigned long getLogPeriod() const { return m_logPeriod; }
     int getBackhaulCount() const { return m_backhaulCount; }
     int getPowerSaveMode() const { return m_powerSaveMode; }
     int getLoggingMode() const { return m_loggingMode; }
     
 private:
     // System configuration
     unsigned long m_logPeriod;
     int m_backhaulCount;
     int m_powerSaveMode;
     int m_loggingMode;
     
     // Internal methods
     bool parseConfiguration(const std::string& config);
     
     // JSON parsing helpers
     std::string extractJsonField(const std::string& json, const std::string& fieldName);
     int extractJsonIntField(const std::string& json, const std::string& fieldName, int defaultValue);
     bool extractJsonBoolField(const std::string& json, const std::string& fieldName, bool defaultValue);
     int findMatchingBracket(const std::string& str, int openPos);
 };
 
 #endif // CONFIGURATION_MANAGER_H