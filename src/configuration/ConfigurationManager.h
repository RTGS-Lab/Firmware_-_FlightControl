// ConfigurationManager.h - Updated to work with vectors
#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "IConfiguration.h"
#include <vector>
#include <memory>
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

class ConfigurationManager : public IConfiguration {
public:
    ConfigurationManager();
    ~ConfigurationManager() = default;
    
    // IConfiguration implementation
    bool setConfiguration(std::string config) override;
    std::string getConfiguration() override;
    //{"config":{"system":{"logPeriod":300,"backhaulCount":4,"powerSaveMode":1,"loggingMode":0,"numAuxTalons":1,"numI2CTalons":1,"numSDI12Talons":1},"sensors":{"numET":0,"numHaar":0,"numSoil":3,"numApogeeSolar":0,"numCO2":0,"numO2":0,"numPressure":0}}}
    std::string getDefaultConfigurationJson() const {
        return "{\"config\":{"
               "\"system\":{"
               "\"logPeriod\":300,"
               "\"backhaulCount\":4,"
               "\"powerSaveMode\":1,"
               "\"loggingMode\":0,"
               "\"numAuxTalons\":1,"
               "\"numI2CTalons\":1,"
               "\"numSDI12Talons\":1"
               "},"
               "\"sensors\":{"
               "\"numET\":0,"
               "\"numHaar\":0,"
               "\"numSoil\":3,"
               "\"numApogeeSolar\":0,"
               "\"numCO2\":0,"
               "\"numO2\":0,"
               "\"numPressure\":0"
               "}"
               "}}";
    }
    int updateSystemConfigurationUid() override;
    int updateSensorConfigurationUid() override;
    
    // System configuration getters
    unsigned long getLogPeriod() const { return m_logPeriod; }
    int getBackhaulCount() const { return m_backhaulCount; }
    int getPowerSaveMode() const { return m_powerSaveMode; }
    int getLoggingMode() const { return m_loggingMode; }
    
    // Sensor count getters
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
    
    // Vector management methods
    void createSensorVectors();
    void clearSensorVectors();
    
    // Static factory methods for creating sensors
    static std::unique_ptr<AuxTalon> createAuxTalon();
    static std::unique_ptr<I2CTalon> createI2CTalon();
    static std::unique_ptr<SDI12Talon> createSDI12Talon();
    static std::unique_ptr<Haar> createHaarSensor();
    static std::unique_ptr<SO421> createO2Sensor(SDI12Talon& talon);
    static std::unique_ptr<SP421> createSolarSensor(SDI12Talon& talon);
    static std::unique_ptr<TDR315H> createSoilSensor(SDI12Talon& talon);
    static std::unique_ptr<Hedorah> createCO2Sensor();
    static std::unique_ptr<T9602> createHumiditySensor();
    static std::unique_ptr<LI710> createETSensor(class ITimeProvider& timeProvider, class ISDI12Talon& talon);
    static std::unique_ptr<BaroVue10> createPressureSensor(SDI12Talon& talon);
    
private:
    // System configuration
    unsigned long m_logPeriod;
    int m_backhaulCount;
    int m_powerSaveMode;
    int m_loggingMode;
    
    // Sensor counts
    int m_numAuxTalons;
    int m_numI2CTalons;
    int m_numSDI12Talons;
    int m_numET;
    int m_numHaar;
    int m_numSoil;
    int m_numApogeeSolar;
    int m_numCO2;
    int m_numO2;
    int m_numPressure;

    int m_SystemConfigUid;
    int m_SensorConfigUid;

    // Internal methods
    bool parseConfiguration(const std::string& config);
    std::string extractJsonField(const std::string& json, const std::string& fieldName);
    int extractJsonIntField(const std::string& json, const std::string& fieldName, int defaultValue);
    bool extractJsonBoolField(const std::string& json, const std::string& fieldName, bool defaultValue);
    int findMatchingBracket(const std::string& str, int openPos);
};

#endif // CONFIGURATION_MANAGER_H