/**
 * @file MockConfiguration.h
 * @brief Mock implementation of the IConfiguration interface for testing
 *
 * © 2025 Regents of the University of Minnesota. All rights reserved.
 */

#ifndef MOCK_CONFIGURATION_H
#define MOCK_CONFIGURATION_H

#include <string>
#include <gmock/gmock.h>
#include "../../src/configuration/IConfiguration.h"

/**
 * @brief Mock implementation of the IConfiguration interface for testing
 */
class MockConfiguration : public IConfiguration {
public:
    MOCK_METHOD(bool, setConfiguration, (std::string config), (override));
    MOCK_METHOD(std::string, getConfiguration, (), (override));
};

#endif // MOCK_CONFIGURATION_H