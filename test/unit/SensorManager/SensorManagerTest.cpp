#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "configuration/SensorManager.h"
#include "MockConfiguration.h"
#include "MockTimeProvider.h"
#include "MockSDI12Talon.h"

class SensorManagerTest : public ::testing::Test {
protected:
    MockConfiguration mockConfig;
    SensorManager sensorManager{mockConfig};
    MockTimeProvider mockTimeProvider;
    MockSDI12Talon mockSDI12Talon;
    
    void SetUp() override {
        // Apply a known configuration
        std::string testConfig = 
            "{\"config\":{"
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
            "\"numET\":1,"
            "\"numHaar\":1,"
            "\"numSoil\":2,"
            "\"numApogeeSolar\":1,"
            "\"numCO2\":1,"
            "\"numO2\":1,"
            "\"numPressure\":1"
            "}"
            "}}";
        
        configManager.setConfiguration(testConfig);
    }
};

// Test initialization of sensors based on configuration
TEST_F(SensorManagerTest, InitializeSensors) {
    // Initialize sensors
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Check that talons were created
    EXPECT_EQ(sensorManager.getAuxTalons().size(), 1);
    EXPECT_EQ(sensorManager.getI2CTalons().size(), 1);
    EXPECT_EQ(sensorManager.getSDI12Talons().size(), 1);
    
    // Check that sensors were created
    auto allSensors = sensorManager.getAllSensors();
    // Expected: 1 ET + 1 Haar + 2 Soil + 1 Solar + 1 CO2 + 1 O2 + 1 Pressure = 8
    EXPECT_EQ(allSensors.size(), 8);
    
    // Check total sensor count (including 3 core sensors)
    EXPECT_EQ(sensorManager.getTotalSensorCount(), 11);
}

// Test clearing all sensors
TEST_F(SensorManagerTest, ClearAllSensors) {
    // Initialize sensors
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Clear all sensors
    sensorManager.clearAllSensors();
    
    // Check that all collections are empty
    EXPECT_EQ(sensorManager.getAuxTalons().size(), 0);
    EXPECT_EQ(sensorManager.getI2CTalons().size(), 0);
    EXPECT_EQ(sensorManager.getSDI12Talons().size(), 0);
    EXPECT_EQ(sensorManager.getAllSensors().size(), 0);
    EXPECT_EQ(sensorManager.getTotalSensorCount(), 3); // Core sensors still counted
}

// Test getting all talons
TEST_F(SensorManagerTest, GetAllTalons) {
    // Initialize sensors
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Get all talons
    auto allTalons = sensorManager.getAllTalons();
    
    // Expected: 1 AuxTalon + 1 I2CTalon + 1 SDI12Talon = 3
    EXPECT_EQ(allTalons.size(), 3);
    
    // Check that all talons are non-null
    for (auto talon : allTalons) {
        EXPECT_NE(talon, nullptr);
    }
}

// Test handling of configuration changes
TEST_F(SensorManagerTest, ConfigurationChanges) {
    // Initialize with current configuration
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Check initial state
    EXPECT_EQ(sensorManager.getAuxTalons().size(), 1);
    EXPECT_EQ(sensorManager.getAllSensors().size(), 8);
    
    // Change configuration
    std::string newConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"logPeriod\":300,"
        "\"backhaulCount\":4,"
        "\"powerSaveMode\":1,"
        "\"loggingMode\":0,"
        "\"numAuxTalons\":2,"
        "\"numI2CTalons\":1,"
        "\"numSDI12Talons\":1"
        "},"
        "\"sensors\":{"
        "\"numET\":0,"
        "\"numHaar\":0,"
        "\"numSoil\":5,"
        "\"numApogeeSolar\":0,"
        "\"numCO2\":0,"
        "\"numO2\":0,"
        "\"numPressure\":0"
        "}"
        "}}";
    
    configManager.setConfiguration(newConfig);
    
    // Re-initialize
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Check that counts have changed
    EXPECT_EQ(sensorManager.getAuxTalons().size(), 2);
    EXPECT_EQ(sensorManager.getAllSensors().size(), 5); // Only soil sensors now
}

// Test sensor and talon relationship
TEST_F(SensorManagerTest, SensorTalonRelationship) {
    // Initialize with a minimal configuration
    std::string minConfig = 
        "{\"config\":{"
        "\"system\":{"
        "\"numAuxTalons\":0,"
        "\"numI2CTalons\":0,"
        "\"numSDI12Talons\":1"
        "},"
        "\"sensors\":{"
        "\"numET\":0,"
        "\"numHaar\":0,"
        "\"numSoil\":1,"
        "\"numApogeeSolar\":0,"
        "\"numCO2\":0,"
        "\"numO2\":0,"
        "\"numPressure\":0"
        "}"
        "}}";
    
    configManager.setConfiguration(minConfig);
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Check that we have one SDI12 talon and one soil sensor
    EXPECT_EQ(sensorManager.getSDI12Talons().size(), 1);
    EXPECT_EQ(sensorManager.getAllSensors().size(), 1);
    
    // The soil sensor should be created using the first SDI12 talon
    // This relationship is maintained internally and can't be easily tested
    // without additional modifications to the code to expose this relationship
}

// Test handling of empty configuration
TEST_F(SensorManagerTest, EmptyConfiguration) {
    // Apply an empty configuration (only system structure, no values)
    std::string emptyConfig = "{\"config\":{\"system\":{},\"sensors\":{}}}";
    configManager.setConfiguration(emptyConfig);
    
    // Initialize sensors
    sensorManager.initializeSensors(mockTimeProvider, mockSDI12Talon);
    
    // Should fall back to defaults
    EXPECT_EQ(sensorManager.getAuxTalons().size(), 1);
    EXPECT_EQ(sensorManager.getI2CTalons().size(), 1);
    EXPECT_EQ(sensorManager.getSDI12Talons().size(), 1);
    
    // Default sensor counts from ConfigurationManager
    auto allSensors = sensorManager.getAllSensors();
    EXPECT_EQ(allSensors.size(), 3); // Default 3 soil sensors
}