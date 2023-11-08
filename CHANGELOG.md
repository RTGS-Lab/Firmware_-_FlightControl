# FIRMWARE
## v2.8.6
### Features
- Added ability for Kestrel to report ALS data for all bands instead of just lux

### Bug Fixes
- Fixed ranging issue which caused ALS report in diagnostic to saturate at 50331 lux

## v2.8.5
### Bug Fixes
- Fixed the erroneous reporting of `0xFFFFFFF7` error code by accel 

## v2.8.4
### Improvements
- Added two stage test for port power fail to differentiate between a port exceeding power limits on its own, or just exceeding the Talon quota 

## v2.8.3
### Features
- Added support for ATMOS22 wind direction sensor (beta)

### Improvements 
- Haar is able to detect and use either hardware I2C configuration automatically
- Added I2C failure detection to T9602

## v2.8.1
### Bug Fixes
- Fixed I2C speed issue which caused bus to default to 100kHz. This caused the O2 init problem 

## v2.8.0
### Improvements
- Added support for auto detection of v1.9 Kestrel logger
- System automatically supports CSA and accel variances between v1.8 and before and v1.9

### Known Bugs
- Apogee O2 sensor fails to init

## v2.7.3
### Improvements
- Added repeated read attempts for Hedorah (tried to read sensor 3 times before failing out, reports error if retry needed at all, then further error if fails out)

## v2.7.2
### Improvements
- Added support for T9602 temp and humidity sensor
- Added support for generic I2C communication error code

## v2.7.0
### Improvements 
- Added support for breaking `data` messages into multiple packets

## v2.6.0
### Improvements
- Reduced SD read/write speed to try to prevent corruption
- Added error reporting for Talon power init 
- Added CSA detect error report
- Added FRAM power down to `ULTRA_LOW_POWER` sleep mode
### Bug Fixes
- Fixed JSON bug for case where CSA fails to detect 

## v2.5.2
### Improvements
- Added fixes to default reading type of MCP3421. No longer waits fixed interval 

## v2.5.1
### Improvements
- Updated layout of user configured elements in code to make configuration easier 

## v2.5.0
### Bug Fixes
- Fixed automatic gain control for small signal analog read from Aux Talon

## v2.4.0
### Features
- Added support for `ULTRA_LOW_POWER` sleep mode

## v2.3.7
### Patches
- Added serial init to Kestrel to prevent sleep problems when not using SDI-12 Talon

## v2.3.6
### Bug Fixes
- Made sleep keep alive/reconnect only performed in `LOW_POWER` mode
### Patches
- Clear RTC trim register on begin - assume erroniously set

## v2.3.5
### Improvements
- Added reading error reporting for Hedorah
- Added more detail to GPS error reporting 
- Added wake time support for sensor class
### Bug Fixes
- Fixed 0 CO2 readings for Hedorah by adding catch/timeout 

## v2.3.4
### Features
- Low power mode now supports sleep times greater than 23 minutes 
### Bug Fixes
- Fixed SDI-12 negative number bug (for SO241 and TDR315H)
- Fixed `takeSample` call to return event type `data/v2` instead of `data`
- Fixed battery light staying on after first log 
- Fixed 'wake up' after first log


## v2.3.2
### Patches
- When RTC enables alarm, it clears the square wave enable bit in case it has been erroniously set (which was happening) and prevents alarm pin from immediately going off

## v2.3.1
### Features
- Added auto accel Z level on startup (if level within +/- 2.5 degrees)

## v2.3.0
### Improvements
- Added forced cell connect time for low power logging modes
- Added GPS support for LOW_POWER mode

## v2.1.3
### Features 
- All of v2.x.y low power dev
- Report TIME_DISAGREE if any time source is out of sync
- Reports more registers from RTC in diagnostic
- Changed publish events to /v2 e.g. "data/v2" to be in lock step with schema v2.x.y update
### Improvements 
- Reports different error code subtype if fail is on a first try or a retry 
### Bug Fixes
- Changed way FAIL_FIND is reported on SDI-12 devices 
	Reports FAIL_FIND only if sensor is not detected, instead of also an SDI-12 fail
- Fix GPS error reporting to be consistent visa-vi GPS_INIT_FAIL vs GPS_READ_FAIL
- Fixed time sync lockup 
- Fixed mislabeling of RTC register in diagnostic report 

## v1.5.3
### Features
- Added forced timesync calls during backhaul events

## v1.5.2
### Features
- Added reporting of RTC config registers
	- Reported by Kestrel lvl 3 diagnostic 
- Added error reporting for failure of RTC wake
	- If RTC fails to wake the device from sleep an ALARM_FAIL error is thrown 

## v1.5.1
### Improvements
- Added separate error code reporting for sensors if disconnected after init
	- If disconnected after init, sensor will report nulls but correct port, will throw `DETECT_FAIL` error code
	- Added on following sensors:
		- SP421
		- TDR315
### Features
- Added type 5 log
	- Logs only most basic diagnostics, data, and errors
	- Logs only to SD
	- Meant for use as an indoor benchtop logger

## v1.5.0
### Bug Fixes
- Dealt with cell reconnect lockup 
	- Enabled threading 
- Fixed backhaul for retry packets
- Fixed SD unsent log dump to eliminate garbage 

### Improvements
- Added basic support for Hedorah CO2 sensor 

## v1.4.1
### Improvements
- Added SD free space reporting to lvl 3 diagnostic 

## v1.4.0
### Improvements 
- Significantly increased startup speed
	- Stores SD file lookup
	- Removed repeated calls to diagnostic and begin
- Diagnostic calls to missing sensors eliminated 
	- SP421, TDR315H
- `PORT_RANGE_ERROR` no longer thrown if sensor is missing
- Added advanced SD diagnostics report
- Added GPS TTFF report
### Bug Fixes
- Corrected GPS reporting so that new point is reported for each packet

## v1.3.1
### Bug Fixes
- Improved GPS status reporting on startup, should only give green light with actual lock now

## v1.3.0
### Features
- Added support/auto detection of Apogee analog sensors 

## v1.2.1
### Bug Fixes
- Fixed accel reading being blank if offset not yet programed 

## v1.2.0
- First deployed version

# SCHEMA
## v2.2.4
- Added Kestrel ALS sensor reporting 

## v2.2.3
- Added ATMOS22 sensor

## v2.2.2
- Added reporting of logger hardware version 

## v2.2.1
- Added T9602 humidity and temp sensor

## v2.2.0
- `data` packets now support being split between two messages

## v2.1.4
- Changed `takeSample` to report event type `data/v2` instead of `data`

## v2.1.3
- Added reporting of log period, backhaul count, power save mode

## v2.1.2
- Changed 'Time Sync' to 'Time Fix'
- Changed CLOCK_NO_SYNC from 0x500103F0 to 0x500300F0

## v2.1.1
- Reports time fix value
- Changed wrapping structure to be compliant with duplicates in DEVICES blob - v2.x.y change
- Reports via "/v2" events 

## v1.2.3
- Added RTC config diagnostic reporting 

## v1.2.2
- Added Hedorah to sensor list

## v1.2.1
- Added SD free space report to lvl 3 diagnostic 

## v1.2.0
- Analog voltage from SDI-12 Talon reported in diagnostic
- Apogee port state (SDI-12/Analog) reported in lvl 3 diagnostic
- SDI-12 Talon now reports data if analog sensor is connected, otherwise null
- Aux Talon error codes fixed 
	- Now all are formal codes
	- Port indication fixed (now 1 - 3, with 0 being unknown/general call)
	- 5v bus reports as port 4
- Fixed sensor reporting if missing, now will report nulls correctly
	- SP421
	- TDR315H
- Added TTFF report for lvl 3 GPS diagnostic 
- Added advanced SD diagnostics for lvl 2 diagnostics 


## v1.1.0
- Accel now reports null properly 