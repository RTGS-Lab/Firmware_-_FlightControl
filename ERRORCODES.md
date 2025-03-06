## Error Code Description 

#### Firmware Version: v2.9.7

### Structure

Each error code is divided as follows 

**0xCcccSSdd**

**C** is the **Class**

**c** is the **Code**

**S** is the **Subtype**

**d** is the **Hardware Device**

### Description

**Base Error Code Hex** - The error code without any modifers 

**Base Error Code Value** - The base error code converted to decimal

**Error Code Structure** - The format of the error code. `e` indicates this character is replaced with a specific error code, `T` indicates this character is replaced with the Talon location of the fault (1,2,3,4), `P` indicates this character is replaced with the port location of the fault (1,2,3,4)

**Class** - Refers to the class of error code, this is the first character. This allows the user to determine the kind of error at a glance See details below.

| **Class** | **Value, Hex** | **Value** |
|:---:|:---:|:---:|
| Unknown | 0 | 0 |
| I2C | 1 | 1 |
| Power | 2 | 2 |
| IO | 3 | 3 |
| Memory | 4 | 4 |
| Timing | 5 | 5 |
| Coms | 6 | 6 |
| Disagree | 7 | 7 |
| Internal  | 8 | 8 |
| Math/Logical | 9 | 9 |
| _unassigned_ | A | 10 |
| _unassigned_ | B | 11 |
| _unassigned_ | C | 12 |
| _unassigned_ | D | 13 |
| System | E | 14 |
| Warning | F | 15 |

**Code** - Refers to the unique value of this code within the class (the 3 characters following the class)

**Subtype** - Refers to the subtype, in some cases a single error will have multiple sub versions

**Hardware Device** - Describes, at a glance, where the error is coming from

| **Hardware Device** | **Hardware Device Code, Hex** |
|:---:|:---:|
| System Wide | 0 |
| Port 1 Talon | 1 |
| Port 2 Talon | 2 |
| Port 3 Talon | 3 |
| Port 4 Talon | 4 |
| _unassigned_ | 5 |
| _unassigned_ | 6 |
| _unassigned_ | 7 |
| _unassigned_ | 8 |
| _unassigned_ | 9 |
| _unassigned_ | A |
| _unassigned_ | B |
| _unassigned_ | C |
| _unassigned_ | D |
| Gonk | E |
| Kestrel | F |

**Hardware Sub-Type** - The sub category of hardware from which the fault originates. This is often used to indicate a Talon/Port combination, but can also be used to indicate specifics on the Kestrel logger

| **Hardware Sub-Device** | **Sub-Hardware Device Code, Hex** |
|:---:|:---:|
| System Wide | 0 |
| Bus Routing | 1 |
| Power | 2 |
| Talon | 3 |
| SD | 4 |
| RTC | 5 |
| Cell | 6 |
| Sensors | 7 |
| GPS | 8 |
| FRAM | 9 |
| Actuation  | A |
| Processor | B |
| _unassigned_ | C |
| _unassigned_ | D |
| _unassigned_ | E |
| _unassigned_ | F |

## List of Error Codes

| **Base Error Code Hex** | **Specific Name** | **Description** | **Base Error Code Value** | **Error Code Structure** | **Class** | **Code** | **Subtype** | **Hardware Device** | **Hardware Sub-Device** | **Code Name** | **Code Location** |
|:---:|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|---|---|
| 0x10010000 | DPS368_INIT_ERROR | Initialization error of the DPS368 sensor  | 268500992 | 0x1001eeTP | I2C | 1 | I2C Error | Talon | Port | DPS368 Init Error | Haar.h |
| 0x10020000 | SHT3X_I2C_ERROR | I2C communication error with the SHT31 sensor | 268566528 | 0x1002eeTP | I2C | 2 | I2C Error | Talon | Port | SHT31 Read Error | Haar.h |
| 0x10030000 | SHT3X_INIT_ERROR | Initialization error of the SHT31 sensor  | 268632064 | 0x100300TP | I2C | 3 |  | Talon | Port | SHT31 Init Error | Haar.h |
| 0x10040100 | SHT3X_NAN_ERROR | Report of NaN from SHT31 for temperature | 268697856 | 0x100401TP | I2C | 4 | 1 | Talon | Port | SHT31 NaN Error | Haar.h |
| 0x10040200 | SHT3X_NAN_ERROR | Report of NaN from SHT31 for humidity | 268698112 | 0x100402TP | I2C | 4 | 2 | Talon | Port | SHT31 NaN Error | Haar.h |
| 0x100500F0 | CSA_INIT_FAIL | Report failure to initialize a CSA sensor | 268763376 | 0x100500F7 | I2C | 5 | CSA | Kestrel | Sensors | PAC1934 init fail | Kestrel.h |
| 0x10060000 | SENSE_ADC_INIT_FAIL | Report failure to initialize a rail ADC sensor on Talons  | 268828672 | 0x1006eeT0 | I2C | 6 | I2C Error | Talon | 0 | MCP3421 init fail | Talon.h |
| 0x10070000 | PORT_ADC_INIT_FAIL | Report failure to initialize a ADCsensor | 268894208 | 0x1007eeT0 | I2C | 7 | I2C Error | Talon | 0 | MCP3221 init fail | Talon.h |
| 0x100800F5 | RTC_EEPROM_READ_FAIL | Report failure to read from RTC EEPROM  | 268959989 | 0x1008eeF5 | I2C | 8 | I2C Error | Kestrel | RTC | MCP79412 EEPROM read fail | MCP79412.h |
| 0x10090000 | TALON_EEPROM_READ_FAIL | Report failure to read from Talon EEPROM  | 269025280 | 0x1009eeT0 | I2C | 9 | I2C Error | Talon | 0 | AT24MAC602 EEPROM read fail | Talon.h |
| 0x100A00F8 | GPS_INIT_FAIL | Report failure to initialize GPS | 269091064 | 0x100AeeF8 | I2C | 10 | I2C Error | Kestrel | GPS | MAX-M8 init failure | Kestrel.h |
| 0x100B00F8 | GPS_READ_FAIL | Report failure to read from GPS | 269156600 | 0x100BeeF8 | I2C | 11 | I2C Error | Kestrel | GPS | MAX-M8 read failure | Kestrel.h |
| 0x100C00F5 | RTC_READ_FAIL | Unable to connect to RTC | 269222133 | 0x100CeeF5 | I2C | 12 | I2C Error | Kestrel | RTC |  | Kestrel.h |
| 0x100C00F7 | ACCEL_INIT_FAIL | Failure to initialize accelerometer on Kestrel | 269222135 | 0x100CeeF7 | I2C | 12 | I2C Error | Kestrel | Sensors |  | Kestrel.h |
| 0x100D00F7 | ACCEL_DATA_FAIL | Failure to read data from accelerometer on Kestrel | 269287671 | 0x100DeeF7 | I2C | 13 | I2C Error | Kestrel | Sensors |  | Kestrel.h |
| 0x100E0000 | AUX_ADC_READ_FAIL | Failure to read from the ADC | 269352960 | 0x100EeeT0 | I2C | 14 | I2C Error | Talon | System Wide | ADS1115 Read failure | AuxTalon.h |
| 0x100F0000 | IO_INIT_FAIL | Failure to initialize IO expander, port corresponds to which IO expander | 269418496 | 0x100FeeTP | I2C | 15 | I2C Error | Talon | Port |  | Talon.h |
| 0x10100000 | AUX_ADC_INIT_FAIL | Failure to initialize ADC on aux talon | 269484032 | 0x1010eeT0 | I2C | 16 | I2C Error | Talon | System Wide | ADS1115 Init failure | AuxTalon.h |
| 0x10110000 | SCD30_INIT_FAIL | Failure to initialize SCD30 CO2 sensor | 269549568 | 0x101100TP | I2C | 17 | 0 | Talon | Port |  | Hedorah.h |
| 0x10120000 | SCD30_I2C_FAIL | Failure to read from SCD30 CO2 sensor | 269615104 | 0x101200TP | I2C | 18 | 0 | Talon | Port |  | Hedorah.h |
| 0x10130000 | CSA_READ_FAIL | Attempt for Talon to read CSA for power test but fail | 269680640 | 0x101300TP | I2C | 19 | 0 | Talon | Port |  | Talon.h |
| 0x101400F7 | ALS_INIT_FAIL | Failure to initialize the ALS on the Kestrel board | 269746423 | 0x1014eeF7 | I2C | 20 | I2C Error Code | Kestrel | Sensor |  | Kestrel.h |
| 0x101500F7 | ALS_DATA_FAIL | Failure to read data from the ALS on the Kestrel board | 269811959 | 0x1015eeF7 | I2C | 21 | 0 | Kestrel | Sensor |  | Kestrel.h |
| 0x101600F7 | CSA_OB_READ_FAIL | Kestrel CSA read failure | 269877495 | 0x10160eF7 | I2C | 22 | CSA | Kestrel | Sensors | PAC1934 read fail | Kestrel.h |
| 0x20010000 | SENSOR_POWER_FAIL | Report of a failure of sensor port on Talon (I2C or Aux) | 536936448 | 0x200100TP | Power | 1 | 0 | Talon | Port | FAULTx indicator | Talon.h |
| 0x20010100 | SENSOR_POWER_FAIL_PERSISTENT | Report of a persistent failure of sensor port on Talon (I2C or Aux) | 536936704 | 0x200101TP | Power | 1 | 1 | Talon | Port | FAULTx indicator | Talon.h |
| 0x20020000 | AUX_POWER_FAIL | Report of a failure of a sensor port on Aux Talon | 537001984 | 0x200200TP | Power | 2 | 0 | Talon | Port | FAULTx indicator | AuxTalon.h |
| 0x20020100 | AUX_POWER_FAIL_PERSISTENT | Report of a failure of a sensor port on Aux Talon | 537002240 | 0x200201TP | Power | 2 | 1 | Talon | Port | FAULTx indicator | AuxTalon.h |
| 0x20030000 | BUS_OUTOFRANGE | General bus out of spec failure | 537067520 | 0x200300TP | Power | 3 | 0 | Talon | Port |  | AuxTalon.h |
| 0x20030100 | BUS_OUTOFRANGE | Bus undervolt | 537067776 | 0x200301TP | Power | 3 | 1 | Talon | Port |  | AuxTalon.h |
| 0x20040000 | SENSOR_POWER_INIT_FAIL | Power to a sensor port failed to initialize  | 537133056 | 0x200400TP | Power | 4 | 0 | Talon | Port |  | Talon.h |
| 0x20040100 | SENSOR_POWER_INIT_FAIL | Power init failed at rising test | 537133312 | 0x200401TP | Power | 4 | 1 | Talon | Port |  | Talon.h |
| 0x20040200 | SENSOR_POWER_INIT_FAIL | Power init failed at steady state test | 537133568 | 0x200402TP | Power | 4 | 2 | Talon | Port |  | Talon.h |
| 0x20050000 | POWER_LOSS_MAIN | Loss of main power | 537198592 | 0x20050000 | Power | 5 | 0 | System Wide | System Wide |  |  |
| 0x20060000 | TALON_POWER_FAIL_EXCESS | Talon has exceeded max power for group (2A) | 537264128 | 0x200600TP | Power | 6 | 0 | Talon | Port |  | Talon.h |
| 0x30010100 | INPUT_BUFF_FAIL | Buffer output fails to go low when forced | 805372160 | 0x300101TP | IO | 1 | 1 | Talon | Port |  | AuxTalon.h |
| 0x30010200 | INPUT_BUFF_FAIL | Buffer fails to go high when forced | 805372416 | 0x300102TP | IO | 1 | 2 | Talon | Port |  | AuxTalon.h |
| 0x30010300 | INPUT_BUFF_FAIL | Buffer fails to go high when ODx released | 805372672 | 0x300103TP | IO | 1 | 3 | Talon | Port |  | AuxTalon.h |
| 0x400100F4 | SD_INIT_FAIL | Failure on initialization of SD card | 1073807604 | 0x400100F4 | Memory | 1 | 0 | Kestrel | SD | SD init fail | KestrelFileHandler.h |
| 0x400200F4 | SD_ACCESS_FAIL | Failure to read/write from SD card | 1073873140 | 0x400200F4 | Memory | 2 | Read/Write | Kestrel | SD | SD r/w fail | KestrelFileHandler.h |
| 0x400300F9 | FRAM_INIT_FAIL | Failure on initialization of FRAM | 1073938681 | 0x400300F9 | Memory | 3 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0x400400F9 | FRAM_ACCESS_FAIL | Failure to read/write from FRAM | 1074004217 | 0x400400F9 | Memory | 3 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0x400400FB | RAM_CRITICAL | More than 90% of RAM is used, calling for shutdown | 1074004219 | 0x400400FB | Memory | 4 | 0 | Kestrel | Processor |  | Kestrel.h |
| 0x400500FB | RAM_FULL | RAM is full such that a variable cannot be allocated, try to reset | 1074069755 | 0x400500FB | Memory | 5 | 0 | Kestrel | Processor |  | Kestrel.h |
| 0x500101F5 | NONREAL_TIME | RTC has been set to non-real time (day or month or year read as zero) | 1342243317 | 0x500101F5 | Timing | 1 | 0 | Kestrel | RTC | Non Real Time | MCP79412.h |
| 0x500201F5 | ANCIENT_TIME | RTC has been set to time before start of 2000 | 1342308853 | 0x500201F5 | Timing | 2 | 1 | Kestrel | RTC | Time Out of Range | MCP79412.h |
| 0x500202F5 | FUTURE_TIME | RTC has been set to impossible future date | 1342309109 | 0x500202F5 | Timing | 2 | 2 | Kestrel | RTC | Time Out of Range | MCP79412.h |
| 0x500300F0 | CLOCK_NO_SYNC | No two clock sources match | 1342374128 | 0x500300F0 | Timing | 3 | 0 | Kestrel | System Wide |  | Kestrel.h |
| 0x500400F0 | CLOCK_UNAVAILABLE | Particle RTC unavailable/reset | 1342439664 | 0x500400F0 | Timing | 4 | 0 | Kestrel | System Wide |  | Kestrel.h |
| 0x500400F5 | CLOCK_UNAVAILABLE | Local time unavailable  | 1342439669 | 0x500400F5 | Timing | 4 | 0 | Kestrel | RTC |  | Kestrel.h |
| 0x500400F6 | CLOCK_UNAVAILABLE | Cell time unavailable | 1342439670 | 0x500400F6 | Timing | 4 | 0 | Kestrel | Cell |  | Kestrel.h |
| 0x500400F8 | CLOCK_UNAVAILABLE | GPS time unavailable  | 1342439672 | 0x500400F8 | Timing | 4 | 0 | Kestrel | GPS |  | Kestrel.h |
| 0x500500F5 | RTC_OSC_FAIL | Local RTC fails to increment  | 1342505205 | 0x500500F5 | Timing | 5 | 0 | Kestrel | RTC |  | Kestrel.h |
| 0x500600F5 | ALARM_FAIL | RTC failed to trigger wakeup alarm | 1342570741 | 0x500600F5 | Timing | 6 | 0 | Kestrel | RTC |  | Kestrel.h |
| 0x54B200F5 | RTC_POWER_LOSS | Power loss reported by local RTC  | 1420951797 | 0x54B200F5 | Timing | 1202 | 0 | Kestrel | RTC |  | MCP79412.h |
| 0x60010000 | SDI12_COM_FAIL | Communication failure (1 = Address out of range, 2 = CRC mismatch, 3 = Ack out of range) | 1610678272 | 0x6001eeTP | Coms | 1 | Fail Type | Talon | Port |  | SDI12Talon.h |
| 0x60020000 | I2C_PORT_FAIL | I2C loopback failed on specified port | 1610743808 | 0x600200TP | Coms | 2 | 0 | Talon | Port |  | I2CTalon.h |
| 0x600300F6 | PUBLISH_FAIL | Connected, but publish failed | 1610809590 | 0x600300F6 | Coms | 3 | 0 | Kestrel | Cell |  | KestrelFileHandler.h |
| 0x700100F5 | CLOCK_MISMATCH | Local mismatch | 1879113973 | 0x700101F5 | Disagree | 1 | 0 | Kestrel | RTC |  | Kestrel.h |
| 0x700100F6 | CLOCK_MISMATCH | Cell mismatch | 1879113974 | 0x700103F6 | Disagree | 1 | 0 | Kestrel | Cell |  | Kestrel.h |
| 0x700100F8 | CLOCK_MISMATCH | GPS mismatch | 1879113976 | 0x700102F8 | Disagree | 1 | 0 | Kestrel | GPS |  | Kestrel.h |
| 0x70020000 | BUS_DISAGREE | Input and output bus readings are out of spec | 1879179264 | 0x700200TP | Disagree | 2 | 0 | Talon | Port |  | AuxTalon.h |
| 0x70030000 | TIME_DISAGREE | At least one time source disagrees with the others | 1879244800 | 0x70030000 | Disagree | 3 | 0 | System Wide | System Wide |  | Kestrel.h |
| 0x80010100 | DPS368_READ_ERROR | Read error of the DPS368 sensor for temperature sensor | 2147549440 | 0x800101TP | Internal | 1 | 1 | Talon | Port | DPS368 Read Error | Haar.h |
| 0x80010200 | DPS368_READ_ERROR | Read error of the DPS368 sensor for pressure sensor | 2147549696 | 0x800102TP | Internal | 1 | 2 | Talon | Port | DPS368 Read Error | Haar.h |
| 0x800300F4 | FILE_LIMIT_EXCEEDED | If file index exceeds the max allowable (9999) throw error | 2147680500 | 0x800300F4 | Internal | 3 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0x800400F4 | FRAM_INDEX_EXCEEDED | If the system tries to use an index that is beyond the structure for the FRAM (corrupted pointer) | 2147746036 | 0x800400F4 | Internal | 4 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0x80050000 | COUNTER_CLEAR_FAIL | Counters did not clear as ordered | 2147811328 | 0x800500TP | Internal | 5 | 0 | Talon | Port |  | AuxTalon.h |
| 0x80060100 | COUNTER_INCREMENT_FAIL | Counter does not increment at all with test of 5 pulese | 2147877120 | 0x800601TP | Internal | 6 | 1 | Talon | Port |  | AuxTalon.h |
| 0x80060200 | COUNTER_INCREMENT_FAIL | Counter does not match on increment of entire range | 2147877376 | 0x800602TP | Internal | 6 | 2 | Talon | Port |  | AuxTalon.h |
| 0x80060300 | COUNTER_INCREMENT_FAIL | Counter increments, but does not match | 2147877632 | 0x800603TP | Internal | 6 | 3 | Talon | Port |  | AuxTalon.h |
| 0x80070000 | ADC_TIMEOUT | New reading not returned in time | 2147942400 | 0x800700T0 | Internal | 7 | 0 | Talon | System Wide |  | AuxTalon.h |
| 0x80090000 | I2C_OB_ISO_FAIL | Internal I2C isolation has failed | 2148073472 | 0x80090000 | Internal | 9 | 0 | Talon | System Wide |  | I2CTalon.h |
| 0x90010100 | SENSOR_PORT_RANGE_FAIL | Sensor port assignment is out of range | 2415984896 | 0x90010100 | Math/Logical | 1 | 1 | 0 | 0 | Port Range Error | Sensor.h |
| 0x90010200 | TALON_PORT_RANGE_FAIL | Talon port assignment is out of range | 2415985152 | 0x90010200 | Math/Logical | 1 | 2 | 0 | 0 | Port Range Error | Sensor.h |
| 0x90010300 | KESTREL_PORT_RANGE_FAIL | Kestrel port assignment is out of range | 2415985408 | 0x90010300 | Math/Logical | 1 | 3 | 0 | 0 | Port Range Error | Kestrel.h |
| 0x900200F0 | OVERSIZE_PACKET | Attempt to write packet to FRAM in excess of max allowable packet size | 2416050416 | 0x900200F0 | Math/Logical | 2 | 0 | Kestrel | System Wide |  | KestrelFileHandler.h |
| 0xA0020000 | TDR315_ERROR | TDR315 reports a failure | 2684485632 | 0xA002eeTP | Sensor | 2 | Sensor Error Code | Talon | Port | TDR315H.h |
| 0xA0040000 | BAROVUE_QUAL_RANGE | The quality metric reported by the sensor indicates intervention | 2684616704 | 0xA0040eTP | Sensor | 4 | Quality Metric | Talon | Port |  | BaroVue10.h |
| 0xE00100F4 | SD_NOT_INSERTED | SD insertion switch has not been pushed - SD card not physically present | 3758162164 | 0xE00100F4 | System | 1 | 0 | Kestrel | SD | CD high | KestrelFileHandler.h |
| 0xE00200FA | WDT_OFF_LEASH | WDT has not been fed, expect a reset  | 3758227706 | 0xE00200FA | System | 2 | 0 | Kestrel | Actuation  |  | Kestrel.h |
| 0xE0030000 | SDI12_SENSOR_MISMATCH | Sensor info mismatch with expected (1 = Address, 2 = report time, 3 = num reports) | 3758292992 | 0xE003eeTP | System | 3 | Fail Type | Talon | Port |  | SDI12Talon.h |
| 0xE0040000 | EXCEED_COLLECT_TIME | Collection time for data was exceeded for a given sensor | 3758358528 | 0xE00400TP | System | 4 | 1 | Talon | Port |  | Sensor.h |
| 0xE0050000 | EXCEED_COLLECT_TIME | Collection time for diagnostics was exceeded for a given sensor | 3758424064 | 0xE00500TP | System | 4 | 2 | Talon | Port |  | Sensor.h |
| 0xE0060000 | EXCEED_COLLECT_TIME | Collection time for metadata was exceeded for a given sensor | 3758489600 | 0xE00600TP | System | 4 | 3 | Talon | Port |  | Sensor.h |
| 0xED000000 | DETECT_FAIL | Failure to detect a sensor which is initialized for a port | 3976200192 | 0xED0000TP | System | 3328 | 0 | Talon | Port |  | Sensor.h |
| 0xEF000000 | TALON_MISSING | Failed to locate an expected Talon | 4009754624 | 0xEF000000 | System | 3840 | 0 | System Wide | System Wide |  | Talon.h |
| 0xF00100F4 | FREE_SPACE_WARNING | Less than 1GB on SD card | 4026597620 | 0xF00100F4 | Warning | 1 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00200F4 | FREE_SPACE_CRITICAL | Less than 100MB on SD card | 4026663156 | 0xF00200F4 | Warning  | 2 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00300F4 | EXPECTED_FILE_MISSING | The expected current file missing from SD | 4026728692 | 0xF00300F4 | Warning | 3 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00400F4 | BASE_FOLDER_MISSING | The base 'GEMS' folder is missing  | 4026794228 | 0xF00400F4 | Warning | 4 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00500F9 | FRAM_OVERRUN | If the system has filled up and tries to use an FRAM location beyond what is allocated | 4026859769 | 0xF00500F9 | Warning | 5 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0xF00600F4 | BACKLOG_PRESENT | SD dump file present | 4026925300 | 0xF00600F4 | Warning | 6 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00700F6 | CELL_FAIL | Fail to connect to cell in general | 4026990838 | 0xF00700F6 | Warning | 7 | 0 | Kestrel | Cell |  | Kestrel.h |
| 0xF00701F6 | CELL_FAIL | Fail to connect to cell on wake | 4026991094 | 0xF00701F6 | Warning | 7 | 1 | Kestrel | Cell |  | Kestrel.h |
| 0xF00800F6 | CLOUD_FAIL | Fail to connect to particle cloud | 4027056374 | 0xF00800F6 | Warning | 8 | 0 | Kestrel | Cell |  | Kestrel.h |
| 0xF00900F4 | FRAM_EXPELLED | FRAM dumped via cell, SD may be inconsistent  | 4027121908 | 0xF00900F4 | Warning | 9 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00A0000 | SYSTEM_RESET | Reports cause of reset when setup is run | 4027187200 | 0xF00Aee00 | Warning | 10 | Reset Reason | System Wide | System Wide |  | Kestrel.h |
| 0xF00B00FB | RAM_LOW | More than 75% of RAM is used | 4027252987 | 0xF00B00FB | Warning | 11 | 0 | Kestrel | Processor |  | Kestrel.h |
| 0xF00C00F8 | GPS_UNAVAILABLE | No GPS lock available, even after wait period | 4027318520 | 0xF00C00F8 | Warning | 12 | 0 | Kestrel | GPS |  | Kestrel.h |
| 0xF00D00F4 | SD_FILE_NOT_FOUND | Expected a file at the stored index, but did not find it or previous | 4027384052 | 0xF00D00F4 | Warning | 13 | 0 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00D01F4 | SD_FILE_NOT_FOUND | Expected a file at the stored index, but did not find it, DID find previous file though | 4027384308 | 0xF00D01F4 | Warning | 13 | 1 | Kestrel | SD |  | KestrelFileHandler.h |
| 0xF00E01F9 | FILE_INDEX_OOR | Stored file index for SD card out of range - greater than max | 4027449849 | 0xF00E01F9 | Warning | 14 | 1 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0xF00E02F9 | FILE_INDEX_OOR | Stored file index for SD card not initialized (NaN read) | 4027450105 | 0xF00E02F9 | Warning | 14 | 2 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0xF00F0000 | COUNTER_OVERFLOW | Counter exceeded max range and has rolled over | 4027514880 | 0xF00F00TP | Warning | 15 | 0 | Talon | Port |  | AuxTalon.h |
| 0xF0100000 | TIME_DELTA_EXCEEDED | Time reading is valid, but an excessive amount of time has elapsed between readings | 4027580416 | 0xF01000T0 | Warning | 16 | 0 | Talon | System Wide |  | AuxTalon.h |
| 0xF0110000 | TIME_BAD | Time provided is not a valid time  | 4027645952 | 0xF01100T0 | Warning | 17 | 0 | Talon | System Wide |  | AuxTalon.h |
| 0xF0120000 | DEVICE_RESET | Device has reset since last read | 4027711488 | 0xF01200T0 | Warning | 18 | 0 | Talon | System Wide |  | AuxTalon.h |
| 0xF01300F0 | WAKE_WARNING | System woke from sleep due to mode other than GPIO | 4027777264 | 0xF01300F0 | Warning | 19 | Wakeup Reason | Kestrel | System Wide |  | Kestrel.h |
| 0xF0140000 | SDI12_READ_FAIL | SDI12 Sensor at port failed to be read | 4027842560 | 0xF01500TP | Warning | 20 | 0 | Talon | Port |  | SDI12Talon.h |
| 0xF01500F5 | TIME_UPDATED | Measured time was outside of max range and updated  | 4027908341 | 0xF01500F5 | Warning | 21 | 0 | Kestrel | RTC |  | Kestrel.h |
| 0xF0160009 | PACKET_LEN_EXCEEDED | Max length of packet exceeded without termination breaks | 4027973641 | 0xF0160009 | Warning | 22 | 0 | System Wide | Math/Logical |  | KestrelFileHandler.h |
| 0xF0170100 | SENSOR_POWER_WARNING | Rising reading was greater than 500mA | 4028039424 | 0xF01700TP | Warning | 23 | 1 | Talon | Port |  | Talon.h |
| 0xF0180000 | FRAM_SPACE_WARNING | FRAM has less than 25% space remaining | 4028104704 | 0xF01800F9 | Warning | 24 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0xF0190000 | FRAM_SPACE_CRITICAL | FRAM has less than 10% space remaining | 4028170240 | 0xF01900F9 | Warning | 25 | 0 | Kestrel | FRAM |  | KestrelFileHandler.h |
| 0xF1000000 | SENSOR_TIMEOUT | Attempt to read sensor timed out for unknown reason | 4043309056 | 0xF10000TP | Warning | 256 | 0 | Talon | Port |  | Sensor.h |
| 0xF1000100 | SENSOR_TIMEOUT | Attempt to read sensor timed out for while waiting for ACK | 4043309312 | 0xF10000TP | Warning | 256 | 1 | Talon | Port |  | Sensor.h |
| 0xF1000200 | SENSOR_TIMEOUT | Attempt to read sensor timed out while waiting for new data | 4043309568 | 0xF10000TP | Warning | 256 | 2 | Talon | Port |  | Sensor.h |
| 0xF1010000 | REPEATED_READ_ATTEMPT | It took more than one read attempt to get a value from this sensor | 4043374592 | 0xF10100TP | Warning | 257 | 0 | Talon | Port |  | Sensor.h |
| 0xF12C0000 | I2C_SENSOR_COM_FAIL | Error has occurred in communication with I2C sensor | 4046192640 | 0xF12CeeTP | Warning | 300 | I2C Error Code | Talon | Port |  | Sensor.h |
| 0xFF000000 | FIND_FAIL | Failed to locate sensor  | 4278190080 | 0xFF000000 | Warning | 3840 | 0 | System Wide | System Wide |  | Sensor.h |