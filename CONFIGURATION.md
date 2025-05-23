### Configuration Management

#### Loading Configuration

Configuration is loaded at startup in the following priority order:

1. **SD Card**: `config.json` file on the SD card
2. **Default**: Built-in default configuration if SD file not found

#### Updating Configuration

Configuration can be updated through:

1. **Cloud Function**: `updateConfig` Particle function
     - copy the intended config.json and paste it as and argument into updateConfig
     - If successful, the device will restart without returning any value.
     - verify that the first metadata packet after reset matches your configuration
     - See below for other types of responses.
3. **SD Card**: Replace `config.json` file and restart system

#### updateConfig Information

#### Configuration UIDs

The system generates unique identifiers for configuration tracking:

- **System Configuration UID**: Changes when system parameters are modified
- **Sensor Configuration UID**: Changes when sensor counts are modified

These UIDs can be retrieved via cloud functions:
- `getSystemConfig`: Returns system configuration UID
- `getSensorConfig`: Returns sensor configuration UID

##### UID Encoding Format

The Configuration UIDs are encoded as 32-bit integers using bit-packing to efficiently store multiple configuration parameters in a single value.

###### System Configuration UID Encoding

The System Configuration UID is constructed using the following bit layout:

```
Bits:  31-16    15-12      11-10       9-8        7-6        5-4        3-2       1-0
Field: logPeriod backhaul  powerSave   logMode    numAux     numI2C     numSDI12  reserved
```

| Field | Bits | Description | Range |
|-------|------|-------------|-------|
| `logPeriod` | 31-16 | Logging period in seconds | 0-65535 |
| `backhaulCount` | 15-12 | Number of logs before backhaul | 0-15 |
| `powerSaveMode` | 11-10 | Power management mode | 0-3 |
| `loggingMode` | 9-8 | Logging behavior mode | 0-3 |
| `numAuxTalons` | 7-6 | Number of Auxiliary Talons | 0-3 |
| `numI2CTalons` | 5-4 | Number of I2C Talons | 0-3 |
| `numSDI12Talons` | 3-2 | Number of SDI-12 Talons | 0-3 |
| Reserved | 1-0 | Reserved for future use | 0-3 |

**Encoding Formula:**
```cpp
int systemUID = (logPeriod << 16) | 
                (backhaulCount << 12) | 
                (powerSaveMode << 10) | 
                (loggingMode << 8) | 
                (numAuxTalons << 6) | 
                (numI2CTalons << 4) | 
                (numSDI12Talons << 2);
```

###### Sensor Configuration UID Encoding

The Sensor Configuration UID uses the following bit layout:

```
Bits:  31-28  27-24   23-20    19-16       15-12   11-8   7-4        3-0
Field: numET  numHaar numSoil  numApogee   numCO2  numO2  numPress   reserved
```

| Field | Bits | Description | Range |
|-------|------|-------------|-------|
| `numET` | 31-28 | Number of ET sensors (LI-710) | 0-15 |
| `numHaar` | 27-24 | Number of Haar atmospheric sensors | 0-15 |
| `numSoil` | 23-20 | Number of soil sensors (TDR315H) | 0-15 |
| `numApogeeSolar` | 19-16 | Number of Apogee solar sensors | 0-15 |
| `numCO2` | 15-12 | Number of CO2 sensors (Hedorah) | 0-15 |
| `numO2` | 11-8 | Number of O2 sensors (SO421) | 0-15 |
| `numPressure` | 7-4 | Number of pressure sensors | 0-15 |
| Reserved | 3-0 | Reserved for future use | 0-15 |

**Encoding Formula:**
```cpp
int sensorUID = (numET << 28) | 
                (numHaar << 24) | 
                (numSoil << 20) | 
                (numApogeeSolar << 16) | 
                (numCO2 << 12) | 
                (numO2 << 8) | 
                (numPressure << 4);
```

##### UID Decoding Examples

###### Decoding System Configuration UID

To extract individual values from a System Configuration UID:

```cpp
// Example UID: 1234567890 (decimal) = 0x499602D2 (hex)
int systemUID = 1234567890;

// Extract each field
int logPeriod = (systemUID >> 16) & 0xFFFF;        // Bits 31-16
int backhaulCount = (systemUID >> 12) & 0xF;       // Bits 15-12
int powerSaveMode = (systemUID >> 10) & 0x3;       // Bits 11-10
int loggingMode = (systemUID >> 8) & 0x3;          // Bits 9-8
int numAuxTalons = (systemUID >> 6) & 0x3;         // Bits 7-6
int numI2CTalons = (systemUID >> 4) & 0x3;         // Bits 5-4
int numSDI12Talons = (systemUID >> 2) & 0x3;       // Bits 3-2
```

###### Decoding Sensor Configuration UID

```cpp
// Example UID: 305419896 (decimal) = 0x12345678 (hex)
int sensorUID = 305419896;

// Extract each field
int numET = (sensorUID >> 28) & 0xF;               // Bits 31-28
int numHaar = (sensorUID >> 24) & 0xF;             // Bits 27-24
int numSoil = (sensorUID >> 20) & 0xF;             // Bits 23-20
int numApogeeSolar = (sensorUID >> 16) & 0xF;      // Bits 19-16
int numCO2 = (sensorUID >> 12) & 0xF;              // Bits 15-12
int numO2 = (sensorUID >> 8) & 0xF;                // Bits 11-8
int numPressure = (sensorUID >> 4) & 0xF;          // Bits 7-4
```

A tool has been developed to help parse this UID and make sense of it, found [in RTGS_Lab gems_sensing_db_tools](https://github.com/RTGS-Lab/gems_sensing_db_tools)

##### Practical Examples

###### Example 1: Default Configuration
```json
{
  "system": {
    "logPeriod": 300,
    "backhaulCount": 4,
    "powerSaveMode": 1,
    "loggingMode": 0,
    "numAuxTalons": 1,
    "numI2CTalons": 1,
    "numSDI12Talons": 1
  }
}
```

**System UID Calculation:**
- logPeriod (300) << 16 = 19660800
- backhaulCount (4) << 12 = 16384
- powerSaveMode (1) << 10 = 1024
- loggingMode (0) << 8 = 0
- numAuxTalons (1) << 6 = 64
- numI2CTalons (1) << 4 = 16
- numSDI12Talons (1) << 2 = 4

**System UID = 19678292** (decimal) or **0x12C4154** (hex)

###### Example 2: Sensor Configuration
```json
{
  "sensors": {
    "numET": 1,
    "numHaar": 2,
    "numSoil": 3,
    "numApogeeSolar": 1,
    "numCO2": 1,
    "numO2": 1,
    "numPressure": 1
  }
}
```

**Sensor UID Calculation:**
- numET (1) << 28 = 268435456
- numHaar (2) << 24 = 33554432
- numSoil (3) << 20 = 3145728
- numApogeeSolar (1) << 16 = 65536
- numCO2 (1) << 12 = 4096
- numO2 (1) << 8 = 256
- numPressure (1) << 4 = 16

**Sensor UID = 305205520** (decimal) or **0x12311110** (hex)

##### UID Usage

Configuration UIDs are used for:

1. **Change Detection**: Compare current UID with stored UID to detect configuration changes
2. **Remote Monitoring**: Cloud functions return UIDs for remote configuration verification
3. **Debugging**: Quick identification of active configuration without full JSON parsing
4. **Optimization**: Fast configuration comparison without string operations
