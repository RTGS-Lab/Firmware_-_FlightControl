# FIRMWARE
## v1.6.0
### Improvements
- Added SDI-12 retry w/CRC on TDR315 and SP421
	- Attempt 3 retry
	- If there is no valid read after 3 tries, throw SDI12_READ_FAIL for sensor
	- Otherwise throw SDI12_COM_FAIL even if just a single error occurs
	- Checks for CRC match, error characters in ACK, etc
### Features
- TDR315 now compatible with TDR315 and TDR310


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