/**
 * @file ConfigurationManager.h
 * @brief Manager for handling device configurations
 *
 * Provides functionality for loading, saving, and applying configurations
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <string>
#include <memory>
#include "IConfiguration.h"

/**
 * @brief Manages configuration settings for the system
 * 
 * Handles loading configuration from default, file or user input,
 * and applies it to the appropriate components
 */
class ConfigurationManager : public IConfiguration {
public:
    /**
     * @brief Constructor
     */
    ConfigurationManager();
    
    /**
     * @brief Destructor
     */
    virtual ~ConfigurationManager();
    
    /**
     * @brief Initialize the configuration system
     * 
     * @param defaultConfig Whether to load the default configuration
     * @return true if initialization succeeded, false otherwise
     */
    bool begin(bool defaultConfig = true);
    
    /**
     * @brief Load configuration from a JSON string
     * 
     * @param config JSON string containing configuration data
     * @return true if configuration was successfully set, false otherwise
     */
    bool setConfiguration(std::string config) override;
    
    /**
     * @brief Get the current configuration as a JSON string
     * 
     * @return std::string JSON representation of the current configuration
     */
    std::string getConfiguration() override;
    
    /**
     * @brief Load the default configuration
     * 
     * @return true if default configuration loaded successfully, false otherwise
     */
    bool loadDefaultConfiguration();
    
    /**
     * @brief Save the current configuration to persistent storage
     * 
     * @return true if saved successfully, false otherwise
     */
    bool saveConfiguration();
    
    /**
     * @brief Apply the configuration to the system
     * 
     * @return true if configuration applied successfully, false otherwise
     */
    bool applyConfiguration();

private:
    std::string m_currentConfig;
};

#endif // CONFIGURATION_MANAGER_H