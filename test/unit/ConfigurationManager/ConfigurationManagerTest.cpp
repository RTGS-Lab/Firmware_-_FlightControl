#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ConfigurationManager.h"

class ConfigurationManagerTest : public ::testing::Test {
protected:
    ConfigurationManager configManager;
    
    void SetUp() override {
        // Reset to known state before each test
    }
};

// Test default configuration
TEST_F(ConfigurationManagerTest, DefaultConfiguration) {
    // Get default configuration
    std::string defaultConfig = configManager.getDefaultConfigurationJson();
    
    // Apply default configuration
    EXPECT_TRUE(configManager.setConfiguration(defaultConfig));
    
    // Check default values
    EXPECT_EQ(configManager.getLogPeriod(), 300); // Default log period
    EXPECT_EQ(configManager.getBackhaulCount(), 4); // Default backhaul count
    EXPECT_EQ(configManager.getPowerSaveMode(), 1); // Default power save mode
    EXPECT_EQ(configManager.getLoggingMode(), 0); // Default logging mode
    
    // Check default talon counts
    EXPECT_EQ(configManager.getNumAuxTalons(), 1);
    EXPECT_EQ(configManager.getNumI2CTalons(), 1);
    EXPECT_EQ(configManager.getNumSDI12Talons(), 1);
    
    // Check default sensor counts
    EXPECT_EQ(configManager.getNumET(), 0);
    EXPECT_EQ(configManager.getNumHaar(), 0);
    EXPECT_EQ(configManager.getNumSoil(), 3);
    EXPECT_EQ(configManager.getNumApogeeSolar(), 0);
    EXPECT_EQ(configManager.getNumCO2(), 0);
    EXPECT_EQ(configManager.getNumO2(), 0);
    EXPECT_EQ(configManager.getNumPressure(), 0);
}

// Test custom configuration
TEST_F(ConfigurationManagerTest, CustomConfiguration) {
    // Create custom configuration
    std::string customConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"logPeriod\":600,"
        "\"backhaulCount\":10,"
        "\"powerSaveMode\":2,"
        "\"loggingMode\":1,"
        "\"numAuxTalons\":2,"
        "\"numI2CTalons\":2,"
        "\"numSDI12Talons\":2"
        "},"
        "\"sensors\":{"
        "\"numET\":1,"
        "\"numHaar\":2,"
        "\"numSoil\":4,"
        "\"numApogeeSolar\":1,"
        "\"numCO2\":1,"
        "\"numO2\":1,"
        "\"numPressure\":1"
        "}"
        "}}";
    
    // Apply custom configuration
    EXPECT_TRUE(configManager.setConfiguration(customConfig));
    
    // Check custom values
    EXPECT_EQ(configManager.getLogPeriod(), 600);
    EXPECT_EQ(configManager.getBackhaulCount(), 10);
    EXPECT_EQ(configManager.getPowerSaveMode(), 2);
    EXPECT_EQ(configManager.getLoggingMode(), 1);
    
    // Check custom talon counts
    EXPECT_EQ(configManager.getNumAuxTalons(), 2);
    EXPECT_EQ(configManager.getNumI2CTalons(), 2);
    EXPECT_EQ(configManager.getNumSDI12Talons(), 2);
    
    // Check custom sensor counts
    EXPECT_EQ(configManager.getNumET(), 1);
    EXPECT_EQ(configManager.getNumHaar(), 2);
    EXPECT_EQ(configManager.getNumSoil(), 4);
    EXPECT_EQ(configManager.getNumApogeeSolar(), 1);
    EXPECT_EQ(configManager.getNumCO2(), 1);
    EXPECT_EQ(configManager.getNumO2(), 1);
    EXPECT_EQ(configManager.getNumPressure(), 1);
}

// Test invalid configuration handling
TEST_F(ConfigurationManagerTest, InvalidConfiguration) {
    // Set default configuration first to have known state
    configManager.setConfiguration(configManager.getDefaultConfigurationJson());
    
    // Try invalid configuration
    std::string invalidConfig = "{\"not_config\":{}}";
    EXPECT_TRUE(configManager.setConfiguration(invalidConfig));
    
    // Values should remain unchanged
    EXPECT_EQ(configManager.getLogPeriod(), 300);
    EXPECT_EQ(configManager.getBackhaulCount(), 4);
}

// Test partial configuration
TEST_F(ConfigurationManagerTest, PartialConfiguration) {
    // Apply default configuration
    configManager.setConfiguration(configManager.getDefaultConfigurationJson());
    
    // Apply partial configuration (only modifying system)
    std::string partialConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"logPeriod\":900,"
        "\"backhaulCount\":8"
        "}"
        "}}";
    
    EXPECT_TRUE(configManager.setConfiguration(partialConfig));
    
    // Check that specified values changed
    EXPECT_EQ(configManager.getLogPeriod(), 900);
    EXPECT_EQ(configManager.getBackhaulCount(), 8);
    
    // Check that unspecified values remain at defaults
    EXPECT_EQ(configManager.getPowerSaveMode(), 1);
    EXPECT_EQ(configManager.getLoggingMode(), 0);
    EXPECT_EQ(configManager.getNumSoil(), 3);
}

// Test configuration UID updates
TEST_F(ConfigurationManagerTest, ConfigurationUIDs) {
    // Apply default configuration
    configManager.setConfiguration(configManager.getDefaultConfigurationJson());
    
    // Get initial UIDs
    int initialSystemUID = configManager.updateSystemConfigurationUid();
    int initialSensorUID = configManager.updateSensorConfigurationUid();
    
    // Modify system configuration
    std::string systemConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"logPeriod\":1200"
        "}"
        "}}";
    
    configManager.setConfiguration(systemConfig);
    
    // System UID should change, sensor UID should remain the same
    int newSystemUID = configManager.updateSystemConfigurationUid();
    int newSensorUID = configManager.updateSensorConfigurationUid();
    
    EXPECT_NE(initialSystemUID, newSystemUID);
    EXPECT_EQ(initialSensorUID, newSensorUID);
    
    // Modify sensor configuration
    std::string sensorConfig = 
        "{\"config\":{"
        "\"sensors\":{"
        "\"numSoil\":5"
        "}"
        "}}";
    
    configManager.setConfiguration(sensorConfig);
    
    // Sensor UID should change
    int finalSensorUID = configManager.updateSensorConfigurationUid();
    EXPECT_NE(newSensorUID, finalSensorUID);
}

// Test configuration serialization
TEST_F(ConfigurationManagerTest, ConfigurationSerialization) {
    // Apply custom configuration
    std::string customConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"logPeriod\":600,"
        "\"backhaulCount\":10,"
        "\"powerSaveMode\":2,"
        "\"loggingMode\":1"
        "}"
        "}}";
    
    configManager.setConfiguration(customConfig);
    
    // Get serialized configuration
    std::string serialized = configManager.getConfiguration();
    
    // Parse it back
    ConfigurationManager newManager;
    newManager.setConfiguration(serialized);
    
    // Values should match
    EXPECT_EQ(newManager.getLogPeriod(), 600);
    EXPECT_EQ(newManager.getBackhaulCount(), 10);
    EXPECT_EQ(newManager.getPowerSaveMode(), 2);
    EXPECT_EQ(newManager.getLoggingMode(), 1);
}

// Test Factory Methods
TEST_F(ConfigurationManagerTest, FactoryMethods) {
    // Test that factory methods create non-null objects
    auto auxTalon = ConfigurationManager::createAuxTalon();
    EXPECT_NE(auxTalon, nullptr);
    
    auto i2cTalon = ConfigurationManager::createI2CTalon();
    EXPECT_NE(i2cTalon, nullptr);
    
    auto sdi12Talon = ConfigurationManager::createSDI12Talon();
    EXPECT_NE(sdi12Talon, nullptr);
    
    auto haarSensor = ConfigurationManager::createHaarSensor();
    EXPECT_NE(haarSensor, nullptr);
    
    // For sensors that need talons, we need to pass in real talons
    auto sdi12TalonObj = ConfigurationManager::createSDI12Talon();
    
    auto o2Sensor = ConfigurationManager::createO2Sensor(*sdi12TalonObj);
    EXPECT_NE(o2Sensor, nullptr);
    
    auto solarSensor = ConfigurationManager::createSolarSensor(*sdi12TalonObj);
    EXPECT_NE(solarSensor, nullptr);
    
    auto soilSensor = ConfigurationManager::createSoilSensor(*sdi12TalonObj);
    EXPECT_NE(soilSensor, nullptr);
    
    auto co2Sensor = ConfigurationManager::createCO2Sensor();
    EXPECT_NE(co2Sensor, nullptr);
    
    auto humiditySensor = ConfigurationManager::createHumiditySensor();
    EXPECT_NE(humiditySensor, nullptr);
    
    // For ET sensor, we need real time provider and SDI12 talon
    // This would require mocking in a real test
    // auto etSensor = ConfigurationManager::createETSensor(timeProvider, sdi12Interface);
    // EXPECT_NE(etSensor, nullptr);
    
    auto pressureSensor = ConfigurationManager::createPressureSensor(*sdi12TalonObj);
    EXPECT_NE(pressureSensor, nullptr);
}