// SensorManager.h - New class to manage sensor vectors
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <vector>
#include <memory>

// Forward declarations
class Sensor;
class Talon;
class AuxTalon;
class I2CTalon;
class SDI12Talon;
class ConfigurationManager;
class ITimeProvider;
class ISDI12Talon;

class SensorManager {
public:
    SensorManager(ConfigurationManager& configManager);
    ~SensorManager() = default;
    
    // Initialize all sensor vectors based on configuration
    void initializeSensors(ITimeProvider& timeProvider, ISDI12Talon& sdi12Interface);
    
    // Initialize only talons
    void initializeTalons();
    
    // Initialize only sensors (after talons are already initialized)
    void initializeSensorsOnly(ITimeProvider& timeProvider, ISDI12Talon& sdi12Interface);
    
    // Clear all sensors
    void clearAllSensors();
    
    // Getters for sensor vectors
    const std::vector<std::unique_ptr<AuxTalon>>& getAuxTalons() const { return auxTalons; }
    const std::vector<std::unique_ptr<I2CTalon>>& getI2CTalons() const { return i2cTalons; }
    const std::vector<std::unique_ptr<SDI12Talon>>& getSDI12Talons() const { return sdi12Talons; }
    
    std::vector<std::unique_ptr<AuxTalon>>& getAuxTalons() { return auxTalons; }
    std::vector<std::unique_ptr<I2CTalon>>& getI2CTalons() { return i2cTalons; }
    std::vector<std::unique_ptr<SDI12Talon>>& getSDI12Talons() { return sdi12Talons; }
    
    // Get all talons as a single vector of base pointers
    std::vector<Talon*> getAllTalons();
    
    // Get all sensors as a single vector (excluding core sensors)
    std::vector<Sensor*> getAllSensors();
    
    // Get total sensor count (including core sensors)
    int getTotalSensorCount() const;
    
private:
    ConfigurationManager& configManager;
    
    // Talon vectors
    std::vector<std::unique_ptr<AuxTalon>> auxTalons;
    std::vector<std::unique_ptr<I2CTalon>> i2cTalons;
    std::vector<std::unique_ptr<SDI12Talon>> sdi12Talons;
    
    // Sensor vectors
    std::vector<std::unique_ptr<class Haar>> haarSensors;
    std::vector<std::unique_ptr<class SO421>> apogeeO2Sensors;
    std::vector<std::unique_ptr<class SP421>> apogeeSolarSensors;
    std::vector<std::unique_ptr<class TDR315H>> soilSensors;
    std::vector<std::unique_ptr<class Hedorah>> gasSensors;
    std::vector<std::unique_ptr<class T9602>> humiditySensors;
    std::vector<std::unique_ptr<class LI710>> etSensors;
    std::vector<std::unique_ptr<class BaroVue10>> pressureSensors;
};

#endif // SENSOR_MANAGER_H