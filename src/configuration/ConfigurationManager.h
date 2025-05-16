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
     int updateSystemConfigurationUid() override;
     int updateSensorConfigurationUid() override;
     
     // System configuration getters
     unsigned long getLogPeriod() const { return m_logPeriod; }
     int getBackhaulCount() const { return m_backhaulCount; }
     int getPowerSaveMode() const { return m_powerSaveMode; }
     int getLoggingMode() const { return m_loggingMode; }
     int getNumAuxTalons() const { return m_numAuxTalons; }
     int getNumI2CTalons() const { return m_numI2CTalons; }
     int getNumSDI12Talons() const { return m_numSDI12Talons; }
     int getNumSoil() const { return m_numSoil; }
     int getNumHaar() const { return m_numHaar; }
     int getNumET() const { return m_numET; }
     int getNumApogeeSolar() const { return m_numApogeeSolar; }
     int getNumCO2() const { return m_numCO2; }
     int getNumO2() const { return m_numO2; }
     int getNumPressure() const { return m_numPressure; }

     
 private:
     // System configuration
     unsigned long m_logPeriod;
     int m_backhaulCount;
     int m_powerSaveMode;
     int m_loggingMode;
     int m_numAuxTalons;
     int m_numI2CTalons;
     int m_numSDI12Talons;
     // Sensor configuration
     int m_numET;
     int m_numHaar;
     int m_numSoil;
     int m_numApogeeSolar;
     int m_numCO2;
     int m_numO2;
     int m_numPressure;

     int m_SystemConfigUid; // Used to track the UID of the system configuration  
     int m_SensorConfigUid; // Used to track the UID of the sensor configuration

     // Internal methods
     bool parseConfiguration(const std::string& config);
     
     // JSON parsing helpers
     std::string extractJsonField(const std::string& json, const std::string& fieldName);
     int extractJsonIntField(const std::string& json, const std::string& fieldName, int defaultValue);
     bool extractJsonBoolField(const std::string& json, const std::string& fieldName, bool defaultValue);
     int findMatchingBracket(const std::string& str, int openPos);
 };
 
 #endif // CONFIGURATION_MANAGER_H