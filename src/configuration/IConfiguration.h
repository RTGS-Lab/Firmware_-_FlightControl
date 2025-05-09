/**
 * @file IConfiguration.h
 * @brief Interface for abstracting configuration functionality
 *
 * Defines a contract for configuring sensors for different applications
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

 #ifndef I_CONFIGURATION_H
 #define I_CONFIGURATION_H
 
 #include <stdint.h>
 #include <string> 

 /**
  * @brief Abstract interface for coinfiguration of sensors
  */
 class IConfiguration {
 public:
     virtual ~IConfiguration() = default;
     virtual bool setConfiguration(std::string config) = 0;
     virtual std::string getConfiguration() = 0;
};
     
 #endif // I_CONFIGURATION_H