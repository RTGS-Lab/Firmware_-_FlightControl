# FlightControl

Frirmware repository for the Flight data logging system.

## Overview

This repository serves as a production and development environment for the Flight data logging system, featuring:

- **Modular Sensor Architecture**: Dynamic sensor configuration and management
- **Hardware Abstraction**: Platform and hardware dependency injection for testability
- **Configuration Management**: JSON-based system and sensor configuration
- **Comprehensive Testing**: Unit tests with Google Test and Google Mock
- **Multiple Platform Support**: Abstracted platform dependencies for Particle devices

## Key Features

- Plug and play sensors
- Support for multiple different protocol configurations including Analog, I2C and SDI12

### Configuration Management

The system supports dynamic configuration through JSON files stored on SD card or applied via cloud functions.

#### Configuration Structure

This is the default configuration if there is no config.json file on the SD card.

```json
{
  "config": {
    "system": {
      "logPeriod": 300,
      "backhaulCount": 4,
      "powerSaveMode": 1,
      "loggingMode": 0,
      "numAuxTalons": 1,
      "numI2CTalons": 1,
      "numSDI12Talons": 1
    },
    "sensors": {
      "numET": 0,
      "numHaar": 0,
      "numSoil": 3,
      "numApogeeSolar": 0,
      "numCO2": 0,
      "numO2": 0,
      "numPressure": 0
    }
  }
}
```

#### System Configuration Parameters

| Parameter | Description | Default | Range |
|-----------|-------------|---------|-------|
| `logPeriod` | Logging interval in seconds | 300 | 0-65535 |
| `backhaulCount` | Number of logs before cellular backhaul | 4 | 1-15 |
| `powerSaveMode` | Power management mode | 1 | 0-3 |
| `loggingMode` | Logging behavior mode | 0 | 0-3 |
| `numAuxTalons` | Number of Auxiliary Talons | 1 | 0-3 |
| `numI2CTalons` | Number of I2C Talons | 1 | 0-3 |
| `numSDI12Talons` | Number of SDI-12 Talons | 1 | 0-3 |

#### Sensor Configuration Parameters

| Parameter | Description | Default | Range |
|-----------|-------------|---------|-------|
| `numET` | Number of Evapotranspiration sensors (LI-710) | 0 | 0-15 |
| `numHaar` | Number of Haar atmospheric sensors | 0 | 0-15 |
| `numSoil` | Number of soil sensors (TDR315H) | 3 | 0-15 |
| `numApogeeSolar` | Number of Apogee solar radiation sensors | 0 | 0-15 |
| `numCO2` | Number of CO2 sensors (Hedorah) | 0 | 0-15 |
| `numO2` | Number of O2 sensors (SO421) | 0 | 0-15 |
| `numPressure` | Number of pressure sensors (BaroVue10) | 0 | 0-15 |

#### Power Save Modes

- **0 - Performance**: No power saving, maximum sensor responsiveness
- **1 - Balanced**: Moderate power saving with good performance
- **2 - Low Power**: Aggressive power saving, longer sensor warm-up times
- **3 - Ultra Low Power**: Maximum power saving, minimal sensor operation

#### Logging Modes

- **0 - Standard**: Regular diagnostic intervals with full data logging
- **1 - Performance**: Maximum data throughput, minimal diagnostics
- **2 - Balanced**: Hourly diagnostics with standard data logging
- **3 - No Local**: Cloud-only logging, no SD card storage

## Hardware Architecture

### Core Components

- **Kestrel Logger**: Main data logging board with GPS, accelerometer, RTC, and cellular connectivity
- **Gonk Battery**: Smart battery management system
- **Talons**: Expansion boards for sensor connectivity
  - **Auxiliary Talon**: General-purpose analog/digital sensor interface
  - **I2C Talon**: I2C sensor interface with power management
  - **SDI-12 Talon**: SDI-12 protocol sensor interface

### Supported Sensors

#### Environmental Sensors
- **LI-710 (ET)**: Evapotranspiration measurements
- **Haar**: Atmospheric temperature, humidity, pressure
- **TDR315H**: Soil moisture and temperature
- **BaroVue10**: Barometric pressure
- **SO421**: Oxygen concentration
- **SP421**: Solar radiation

#### Gas Sensors
- **Hedorah**: CO2 concentration with SCD30 sensor
- **T9602**: Humidity and temperature

## Development Setup

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler
- Git with submodule support

### Building Tests

```bash
# Clone repository with submodules
git clone --recursive https://github.com/RTGS-Lab/Firmware_-_FlightControl.git
cd Firmware_-_FlightControl

# Create build directory
mkdir build && cd build

# Configure and build tests
cmake ..
make

# Run tests
./test/unit_tests
```

### Project Structure

```
FlightControl-Demo/
├── src/                          # Source code
│   ├── configuration/            # Configuration management
│   ├── hardware/                 # Hardware interface implementations
│   └── platform/                 # Platform abstraction implementations
├── test/                         # Unit tests
│   ├── mocks/                    # Mock implementations
│   └── unit/                     # Unit test files
├── lib/                          # External libraries (git submodules)
└── docs/                         # Documentation
```

## Testing Framework

### Unit Testing

The project uses Google Test and Google Mock for unit testing:

- **Platform Abstraction Testing**: Mock implementations for all platform dependencies
- **Hardware Interface Testing**: Mock hardware components for isolated testing
- **Configuration Testing**: Validation of configuration parsing and management
- **Sensor Management Testing**: Dynamic sensor initialization and management

### Mock Architecture

Mock implementations are provided for:

- **Platform Dependencies**: TimeProvider, GPIO, System, Wire, Cloud, Serial
- **Hardware Components**: IO Expanders, Current Sensors, RTC, GPS, Accelerometer
- **Sensor Interfaces**: SDI-12, I2C communication protocols

### Running Tests

```bash
# Run all tests
./test/unit_tests

# Run specific test suites
./test/unit_tests --gtest_filter="ConfigurationManagerTest.*"
./test/unit_tests --gtest_filter="KestrelTest.*"
./test/unit_tests --gtest_filter="SensorManagerTest.*"
```

## Configuration Examples

### Full Environmental Station

```json
{
  "config": {
    "system": {
      "logPeriod": 300,
      "backhaulCount": 8,
      "powerSaveMode": 1,
      "loggingMode": 0,
      "numAuxTalons": 1,
      "numI2CTalons": 1,
      "numSDI12Talons": 1
    },
    "sensors": {
      "numET": 1,
      "numHaar": 1,
      "numSoil": 2,
      "numApogeeSolar": 0,
      "numCO2": 0,
      "numO2": 0,
      "numPressure": 0
    }
  }
}
```

## Cloud Functions

The system exposes several Particle cloud functions:

- `updateConfig`: Update system configuration
- `getSystemConfig`: Get system configuration UID
- `getSensorConfig`: Get sensor configuration UID
- `nodeID`: Set custom node identifier
- `findSensors`: Trigger sensor auto-detection
- `findTalons`: Trigger Talon auto-detection
- `systemRestart`: Restart the system
- `takeSample`: Force immediate data collection
- `commandExe`: Execute system commands

## Schema and Error Codes

- **Data Schema**: SEE SCHEMA.md
- **Error Codes**: SEE ERRORCODES.md
- **Command Execution**: SEE COMMANDEXE.md
- **System and Sensor Configuration**: SEE CONFIGURATION.md

## Contributing

1. Fork the repository
2. Create a feature branch (feature/name-of-branch)
3. Add unit tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## Related Repositories

This project depends on several driver libraries available as git submodules:

- Driver libraries for individual sensors
- Hardware abstraction libraries
- Platform dependency interfaces
- Communication protocol implementations

For a complete list, see [.gitmodules](.gitmodules).