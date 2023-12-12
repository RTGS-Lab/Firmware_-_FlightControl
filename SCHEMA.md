## Schema Description 

#### Schema Version: v2.2.6

| **Key Name** | **Definition** | **Parent Key** | **Device** | **Diagnostic Level** | **Range, Min** | **Range, Max** | **Expected Value** |
|---|---|---|---|---|---|---|---|
| StackPointer | Pointer for write to the FRAM | File | File | 4 | 0 | 65536 | Greater than zero |
| FRAM_Util | Percent of FRAM which is occupied with data | File | File | 4 | 0 | 100 | Less than 75% |
| Pos | Position of 'file' report | File | File | N/A | N/A | N/A | null |
| PORT_1 | Group of information about port 1 on the Aux Talon | Talon-Aux | Aux Talon | 4 | N/A | N/A | N/A |
| PORT_2 | Group of information about port 2 on the Aux Talon | Talon-Aux | Aux Talon | 4 | N/A | N/A | N/A |
| PORT_3 | Group of information about port 3 on the Aux Talon | Talon-Aux | Aux Talon | 4 | N/A | N/A | N/A |
| Vi | Input voltage to the specified port, measured in millivolts. Expectation depends on 3.3V/5V setting. Should be 3.3V if PVset = 0, should be 5V if PVset = 1 | PORT_X | Aux-Talon | 4 | 0 | 6144 | 3300/5000 &plusmn; 5% |
| Vo | Output voltage to the specified port, measured in millivolts. Should be the same as Vi unless there is an issue | PORT_X | Aux-Talon | 4 | 0 | 6144 | 3300/5000 &plusmn; 5% |
| PVset | Indicates if port is running at 3.3V or 5V. Configured by DIP switch on board Talon | PORT_X | Aux-Talon | 4 | 0 | 1 | 0/1 |
| OVF | Indicates a counter overflow on the given port - this indicates a potential loss of data if the counter value from this port is relevent | PORT_X | Aux-Talon | 4 | 0 | 1 | 0 |
| Fault | Indicates a power fault has occurred with the counter element | PORT_X | Aux-Talon | 4 | 0 | 1 | 0 |
| 5V_BUS | The voltage of the 5V rail measured in millivolts  | Talon-Aux | Aux-Talon | 4 | 0 | 6144 | 5000 &plusmn; 5% |
| ALPHA | Decimal value which corresponds to the binary port value of the alpha IO expander | Talon-Aux | Aux-Talon | 4 | 0 | 65535 | 6143 |
| BETA | Decimal value which corresponds to the binary port value of the beta IO expander | Talon-Aux | Aux-Talon | 4 | 0 | 65535 | 58296 |
| I2C | A list of all I2C addresses (in decimal form) which are detected on the Aux Talon bus | Talon-Aux | Aux-Talon | 4 | N/A | N/A | 0,32,35,36,48,73,80,88 |
| PORT_V | List of the voltages read at each port, used to confirm valid power to sensors. Value reported in volts. | Talon-I2C | I2C-Talon | 4 | 0 | 8.192 | 3.3 &plusmn; 5% |
| PORT_I | List of the currents measured at each port, used to confirm presance of sensors and correct function. Value reported in milliamps | Talon-I2C | I2C-Talon | 4 | 0 | 999 | Sensor dependent |
| ALPHA | Decimal value which corresponds to the binary port value of the alpha IO expander | Talon-I2C | I2C-Talon | 4 | 0 | 65535 | 8207 |
| ALPHA_INT | Decimal value which corresponds to the binary value of the interrupt registers on the alpha IO expander. When an interrupt is triggered, the bit is cleared | Talon-I2C | I2C-Talon | 4 | 0 | 65535 | 65535 |
| MUX | Decimal value which corresponds to the binary value of the IO expander which controls the MUX, low level firmware diagnostic. | Talon-I2C | I2C-Talon | 4 | 0 | 15 | 15 |
| I2C_OB | A list of all I2C addresses (in decimal form) which are detected on the I2C Talon internal bus | Talon-I2C | I2C-Talon | 4 | N/A | N/A | 0,34,48,65,80,88,107 |
| I2C_1 | A list of all I2C addresses (in decimal form) which are detected on the port 1 bus | Talon-I2C | I2C-Talon | 4 | N/A | N/A | N/A |
| I2C_2 | A list of all I2C addresses (in decimal form) which are detected on the port 2 bus | Talon-I2C | I2C-Talon | 4 | N/A | N/A | N/A |
| I2C_3 | A list of all I2C addresses (in decimal form) which are detected on the port 3 bus | Talon-I2C | I2C-Talon | 4 | N/A | N/A | N/A |
| I2C_4 | A list of all I2C addresses (in decimal form) which are detected on the port 4 bus | Talon-I2C | I2C-Talon | 4 | N/A | N/A | N/A |
| PORT_V | List of the voltages read at each port, used to confirm valid power to sensors. Value reported in volts. | Talon-SDI12 | SDI12-Talon | 4 | 0 | 24.576 | 12 &plusmn; 10% |
| PORT_I | List of the currents measured at each port, used to confirm presance of sensors and correct function. Value reported in milliamps | Talon-SDI12 | SDI12-Talon | 4 | 0 | 999 | Sensor dependent |
| Apogee_V | Analog voltage reading of the analog pin in the Apogee port (port 4), measured in volts | Talon-SDI12 | SDI12-Talon | 4 | 0 | 5 | N/A |
| ALPHA | Decimal value which corresponds to the binary port value of the alpha IO expander | Talon-SDI12 | SDI12-Talon | 4 | 0 | 65535 | 28687 |
| ALPHA_INT | Decimal value which corresponds to the binary value of the interrupt registers on the alpha IO expander. When an interrupt is triggered, the bit is cleared | Talon-SDI12 | SDI12-Talon | 4 | 0 | 65535 | 65535 |
| MUX | Decimal value which corresponds to the binary value of the IO expander which controls the MUX, low level firmware diagnostic. | Talon-SDI12 | SDI12-Talon | 4 | 0 | 15 | 15 |
| I2C_OB | A list of all I2C addresses (in decimal form) which are detected on the SDI-12 Talon internal bus | Talon-SDI12 | SDI12-Talon | 4 | N/A | N/A | 0,37,48,65,77,80,88,107 |
| ADRs | A list of the address detected at each port on the SDI-12 Talon, report 'null' if no address detected | Talon-SDI12 | SDI12-Talon | 4 | 0 | 9 | N/A |
| Cycles | Number of total cycles of the battery for the system | GONK | Gonk board | 2 | 0 | 655.35 | N/A |
| CellV | Instantaneous voltage of the battery bus, reported in millivolts | GONK | Gonk board | 4 | 0 | 5119.92 | 2500 to 4200 |
| CellVAvg | Averaged (short interval) voltage of the battery, reported in millivolts | GONK | Gonk board | 4 | 0 | 5119.92 | 2500 to 4200 |
| CapLeft | Modeled remaining capacitance of the battery, reported in milliamphours (mAh) | GONK | Gonk board | 4 | 0 | 32767.5 | N/A |
| CapTotal | Modeled total capacitance of the battery, reported in milliamphours (mAh) | GONK | Gonk board | 4 | 0 | 32767.5 | 10200 |
| TTF | Number of seconds (at current charge rate) until the logger is fully charged  | GONK | Gonk board | 4 | 0 | 368634.24 | N/A |
| SoC | Modeled State of Charge (SoC) of the battery pack, reported in percent | GONK | Gonk board | 4 | 0 | 256 | Less than 100% |
| Temperature  | Temperature measured on the battery board, reported in °C | GONK | Gonk board | 4 | -40 | 127.996 | N/A |
| PORT_V | List of the voltages read at each node on main Kestrel board, used to confirm valid power to subsystems. Value reported in volts. Values: VBat, VSys, VPrime, VSolar, VBulk, 3v3Core, 3v3Bulk, 3v3Talon | Kestrel | Kestrel | 4 | 0 | 32 | N/A |
| PORT_I | List of the currents read at each node on main Kestrel board, used to confirm valid power to subsystems. Value reported in milliamps. Values: IBat, ISys, IPrime,ISolar, IBulk, I_3v3Core, I_3v3Bulk, I_3v3Talon | Kestrel | Kestrel | 4 | -50000 | 50000 [^1]| N/A |
| AVG_P | List of averages reported by the first set of power measurements (Bat, Sys, Prime, Solar). Power is averaged on a daily period, last reset reported in `LAST_CLR`. Reported in Watts | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| LAST_CLR | Unix time when the `AVG_P` values were last reset | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| ALS | Ambient light value reported by the on board light sensor. This value is reported in Lux, but is measured inside of the logger box, so may be affected by the box lid. Used to gauge solar presence, etc | Kestrel | Kestrel | 4 | 0 | 150022 [^2] | N/A |
| RH | Relative humidity measured inside of logger box, reported in percent RH. Used to diagnose when desiccant needs to be replaced, etc | Kestrel | Kestrel | 4 | 0 | 100 | <50 |
| ACCEL | Accelerometer values which provide the orientation of the data logger (X, Y, Z) - reported in g values | Kestrel | Kestrel | 4 | -1 | 1 | N/A |
| SIV | Number of satellites which are in view from the GPS system | Kestrel | Kestrel | 4 | 0 | 24 | >3 |
| FIX | Describes the type of GPS fix which is available (0=no fix, 1=dead reckoning, 2=2D, 3=3D, 4=GNSS, 5=Time fix) | Kestrel | Kestrel | 4 | 0 | 5 | 3 |
| Temperature  | Reported temperature from different devices on board the main logger board (Temp from RH sensor, Temp from accel sensor), reported in °C | Kestrel | Kestrel | 4 | -40 | 125 | N/A |
| Free Mem | Amount of free RAM in bytes on the Particle processor   | Kestrel | Kestrel | 4 | 0 | 256000 | >80000 |
| Time Fix | Reports the quality of the time fix the device currently has - described how trusted the time should be (4 = GPS and Cell present, 3 = GPS OR Cell present, 2 = only local sources, 1 = only delta time agrees - bad, 0 = not even delta agrees, no fix - very bad)  | Kestrel | Kestrel | 4 | 0 | 4 | 4 |
| Time Source | A list of which time sources are used to determine the current time - chooses two 'best' available  | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| Times | A list of each time source and what it reported when last read, all reported in Unix time | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| LOCAL | The time which is kept on board the Particle unit and is returned when any general purpose time is used, reported in Unix format | Times | Kestrel | 4 | N/A | N/A | N/A |
| GPS | Time from the GPS satellite system, reported in Unix format | Times | Kestrel | 4 | N/A | N/A | N/A |
| CELL | Time from the cellular network, acquired through the Particle module, reported in Unix format | Times | Kestrel | 4 | N/A | N/A | N/A |
| GPS_RTC | Time from the RTC on board the GPS, used as a separate on board time source (with battery backup). Also needed because GPS does not request a new time from satellite every logging cycle. reported in Unix format | Times | Kestrel | 4 | N/A | N/A | N/A |
| RTC | Time from the hardware RTC on board the logger, reported in Unix format  | Times | Kestrel | 4 | N/A | N/A | N/A |
| INC | Last agreed upon time with an increment of the seconds believed (by the micro) to have passed since then - least assured time. Reported in Unix format | Times | Kestrel | 4 | N/A | N/A | N/A |
| Last Sync | The time at which all times were last collected and synchronized, reported in Unix format | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| OB | Decimal value which corresponds to the binary port value of the IO expander used for 'on-board' functions | Kestrel | Kestrel | 4 | 0 | 65535 | N/A |
| Talon | Decimal value which corresponds to the binary port value of the IO expander used for 'Talon' related functions | Kestrel | Kestrel | 4 | 0 | 65535 | 34953 |
| I2C | A list of all I2C addresses (in decimal form) which are detected on the Kestrel main logger internal bus | Kestrel | Kestrel | 4 | N/A | N/A | N/A |
| Files | A list of the full names of all of the files used for data recording  | File | File | 2 | N/A | N/A | N/A |
| SD_Size | The self reported size of the SD card in kB, used to make sure correct SD card is installed  | File | File | 2 | 0 | N/A | 16000/32000 [^3] |
| SD_Free | The self reported amount of free space still left on the SD card, reported in kB, used to make sure adequate space is left on the SD for logging | File | File | 2 | 0 | `SD_Size` | >0.5*`SD_Size` |
| SD_SN | The serial number which is read from the SD card - used to keep track of individual cards should the need arrise  | File | File | 2 | N/A | N/A | N/A |
| SD_MFG | Code which is reported by the SD card which corresponds to the manufacturer (see details in link) - common values: SanDisk = 3, Kingston = 65, Samsung = 27  | File | File | 2 | 0 | 255 | 3 [^4] |
| SD_TYPE | Code which is reported by the SD card which corresponds to the kind of SD card - common values: SCSDv1 = 1, SCSDv2 = 2, SDHC = 3 (kind generally used)] | File | File | 2 | 0 | 3 | 3 |
| In_Occ | Denotes if the digital input is occupied or not. If there is an external source controlling the state of the digital line, the bit is set, if not, it is cleared. Note: This tests digital pins only, not open drain pins | PORT_X | Aux-Talon | 2 | 0 | 1 | N/A |
| In_State | Denotes the current state of the digital input line | PORT_X | Aux-Talon | 2 | 0 | 1 | N/A |
| Ain_O | Voltage reading of the analog line with no load applied (Open), reported in millivolts | PORT_X | Aux-Talon | 2 | 0 | 6144 | N/A |
| Ain_D | Voltage reading of the analog line with stray charge removed (Drained), reported in millivolts | PORT_X | Aux-Talon | 2 | 0 | 6144 | N/A |
| Ain_L | Voltage reading of the analog line with fixed load applied (Loaded), reported in millivolts  | PORT_X | Aux-Talon | 2 | 0 | 6144 | N/A |
| Accel_Offset | Offset which is applied to the system when level. This value is subtracted from the raw accelerometer outputs to get a proper 0 when leveled. Values reported in g's  | Kestrel | Kestrel | 2 | -1 | 1 | N/A |
| RTC_Config | The reported output of several registers on the on board RTC. This is specifically to address an issue with configuration being erroneously overwritten. People who are not Bobby Schulz - pay this no mind. | Kestrel | Kestrel | 2 | -1 | 1 | N/A |
| TTFF | How long (in milliseconds) it took to get the most recent GPS fix | Kestrel | Kestrel | 2 | 0 | N/A | <30000 |

[^1]: Per channel min/max is as follow 

	0 - Min: -50000, Max: 50000

	1 - Min: 0, Max: 50000
	
	2 - Min: 0, Max: 50000
	
	3 - Min: 0, Max: 50000
	
	4 - Min: 0, Max: 50000
	
	5 - Min: 0, Max: 10000
	
	6 - Min: 0, Max: 10000
	
	7 - Min: 0, Max: 10000

[^2]: For firmware <`v2.9.5` this value saturated at 50331
[^3]: This is for 16GB and 32GB cards respectively, which are the most commonly used 
[^4]: This is the MFG code for SanDisk SD cards, which are the only SD cards we recommend using