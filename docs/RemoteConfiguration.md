# Remote Configuration Guide

The FlightControl-Demo firmware supports remote configuration updates through Particle functions. This allows you to change the system configuration and enabled sensors without requiring a firmware update.

## Configuration Overview

The firmware can be configured using a JSON configuration format. The configuration includes:

1. **System settings**: Controls logging periods, backhaul frequency, power save modes, etc.
2. **Sensors list**: Defines which sensors are enabled and their connection details

## JSON Format

The configuration uses the following format:

```json
{
  "config": {
    "system": {
      "logPeriod": 300,
      "backhaulCount": 4,
      "powerSaveMode": 1,
      "loggingMode": 0
    },
    "sensors": [
      {
        "type": "FileSystem",
        "enabled": true
      },
      {
        "type": "Haar",
        "enabled": true,
        "port": 1,
        "talonPort": 1,
        "version": "0x20"
      }
    ]
  }
}
```

### System Parameters

- `logPeriod`: Time in seconds between logging events (default: 300)
- `backhaulCount`: Number of log events before a backhaul is performed (default: 4)
- `powerSaveMode`: Power saving mode (0=Performance, 1=Low Power, 2=Ultra Low Power)
- `loggingMode`: Logging mode (0=Standard, 1=Performance, 2=Balanced, 3=No Local)

### Sensor Parameters

- `type`: The sensor type identifier (required)
- `enabled`: Whether the sensor is enabled (true/false)
- `port`: The sensor port on the Talon (only for non-core sensors)
- `talonPort`: The Talon port on the Kestrel (only for non-core sensors)
- `version`: The hardware version, in decimal or hex (e.g., "0x20")

## Core Sensors

The following sensors are considered "core" sensors and are always available:

- FileSystem
- Aux
- I2C
- SDI12
- Battery
- Logger

## Dynamic Sensors

The following sensors can be configured dynamically:

- Haar
- Hedorah
- T9602
- TDR315H (up to 3 instances)
- TEROS11 (up to 2 instances)
- LI710
- SO421
- SP421
- ATMOS22
- BaroVue10

## Updating Configuration Remotely

To update the configuration remotely, call the Particle function `updateConfig` with a valid JSON configuration string. The function can be called using the Particle CLI or the Particle Cloud API.

### Using Particle CLI

```bash
particle call <your-device-id> updateConfig <configuration-json>
```

### Return Codes

- `1`: Success
- `-1`: Failed to update configuration
- `-2`: Invalid JSON format

## Configuration Files

Sample configuration files can be found in the `test/sample_configurations/` directory:

- `standard_config.json`: The default configuration
- `minimal_config.json`: A minimal configuration with fewer sensors
- `full_config.json`: A configuration with many sensors

## Reset Process

When a new configuration is applied:

1. The system settings are updated
2. All non-core sensors are reset
3. Talons and sensors are re-detected
4. Sensors are re-initialized

This ensures that the device applies the new configuration completely.

## Persistent Storage

The current configuration is stored on the SD card as `config.json`. If the device reboots, it will load this configuration during startup. If no configuration file exists or there's an error loading it, the device will use the default configuration.