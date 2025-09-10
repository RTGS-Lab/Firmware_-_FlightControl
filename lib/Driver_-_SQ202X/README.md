# Apogee SQ-202X Quantum Sensor Driver

Driver for Apogee SQ-202X quantum sensors connected to SDI12Talon port 4.

## Overview

This driver supports the Apogee SQ-202X quantum sensor that connects to the dedicated Apogee port (port 4) on the SDI12Talon board. The driver automatically detects if the port is in analog mode (no SDI-12 signals detected) and converts voltage readings from the built-in MCP3221 ADC to PPFD (µmol m⁻² s⁻¹) using the SQ-202X calibration factor.

## Features

- Automatic detection of analog vs SDI-12 mode
- SQ-202X specific calibration (1.6 µmol m⁻² s⁻¹ per mV)
- PPFD output in proper scientific units
- Voltage readings in both V and mV
- Compatible with existing sensor framework
- Error handling and diagnostics

## Hardware Requirements

- SDI12Talon board with port 4 configured for analog input
- Apogee SQ-202X quantum sensor connected to port 4
- No SDI-12 devices on port 4 (will override analog mode)

## Usage

The driver is integrated into the sensor management system and configured via JSON configuration files.

© 2023 Regents of the University of Minnesota. All rights reserved.