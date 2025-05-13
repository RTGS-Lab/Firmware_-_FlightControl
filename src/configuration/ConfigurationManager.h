/**
 * @file ConfigurationManager.h
 * @brief Configuration manager for sensor deployment
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef CONFIGURATION_MANAGER_H
 #define CONFIGURATION_MANAGER_H
 
 #include "IConfiguration.h"
 #include <Sensor.h>
 #include <Talon.h>
 // These are for pre-allocation of sensors
 #include <Haar.h>
 #include <Hedorah.h>
 #include <TDR315H.h>
 #include <Li710.h>
 #include <SO421.h>
 #include <SP421.h>
 #include <ATMOS22.h>
 #include <TEROS11.h>
 #include <T9602.h>
 #include <BaroVue10.h>
 
 class ConfigurationManager : public IConfiguration {
 public:
     // Constants for fixed allocation
     static const uint8_t MAX_TOTAL_SENSORS = 20; // Maximum number of sensors (core + dynamic)
     static const uint8_t MAX_DYNAMIC_SENSORS = 14; // Maximum number of dynamic sensors
 
     ConfigurationManager();
     ~ConfigurationManager() = default; // No dynamic allocation, no need for custom destructor
     
     // IConfiguration implementation
     bool setConfiguration(std::string config) override;
     std::string getConfiguration() override;
     
     // Sensor pool access
     Sensor** getSensorArray();
     uint8_t getSensorCount() const;
     
     // Method to register available sensors (called once at startup)
     void registerAvailableSensors(Sensor** availableSensors, uint8_t count);
     
     // Method to reset all sensors (clears their allocation)
     void resetSensors();
     
     // System configuration getters
     unsigned long getLogPeriod() const { return m_logPeriod; }
     int getBackhaulCount() const { return m_backhaulCount; }
     int getPowerSaveMode() const { return m_powerSaveMode; }
     int getLoggingMode() const { return m_loggingMode; }
     
 private:
     // Fixed sensor array for all sensors (core + dynamic)
     Sensor* m_sensorPool[MAX_TOTAL_SENSORS];
     uint8_t m_sensorCount;
     
     // Array to store pointers to available sensors (both core and optional)
     Sensor** m_availableSensors;
     uint8_t m_availableSensorCount;
     
     // Array to track which sensors are enabled
     bool m_sensorEnabled[MAX_TOTAL_SENSORS];
     
     // Pre-allocated sensors for dynamic configuration
     // These sensors are declared here but initialized on demand
     Haar m_haar;
     Hedorah m_hedorah;
     T9602 m_t9602;
     TDR315H m_tdr315h[3]; // Allow up to 3 soil sensors
     TEROS11 m_teros11[2]; // Allow up to 2 TEROS11 sensors
     LI710 m_li710;
     SO421 m_so421;
     SP421 m_sp421;
     ATMOS22 m_atmos22;
     BaroVue10 m_barovue;
     // Add other sensor types as needed
     
     // Flags to track which pre-allocated sensors are in use
     bool m_dynamicSensorInUse[MAX_DYNAMIC_SENSORS];
     
     // System configuration
     unsigned long m_logPeriod;
     int m_backhaulCount;
     int m_powerSaveMode;
     int m_loggingMode;
     
     // Internal methods
     void clearSensorPool();
     bool parseConfiguration(const std::string& config);
     bool configureSensor(const std::string& type, int port, int talonPort, int version);
     bool enableCoreSensor(const std::string& type);
     Sensor* findAvailableSensorByType(const std::string& type);
     
     // JSON parsing helpers
     std::string extractJsonField(const std::string& json, const std::string& fieldName);
     int extractJsonIntField(const std::string& json, const std::string& fieldName, int defaultValue);
     bool extractJsonBoolField(const std::string& json, const std::string& fieldName, bool defaultValue);
     int findMatchingBracket(const std::string& str, int openPos);
 };
 
 #endif // CONFIGURATION_MANAGER_H