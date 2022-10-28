/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.h
 - Class / enums / API

 Updates:
 - 2019-07-14:
  	  - Rewrite of read - cleaner.
  	  - Target labels extendible with enum and PROGMEM strings
  	  - Retired copy_raw_to_serial0() code - use VE_DUMP on read
  	  - Added some tunable parameters see #defines
******************************************************************/

#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include <Arduino.h>

// Tunable parameters - defaults tested on mega2560 R3
#define VED_LINE_SIZE 30		 // Seems to be plenty. VE.Direct protocol could change
#define VED_MAX_LEBEL_SIZE 7	 // Max length of all labels of interest + '\0'. See ved_labels[]
#define VED_MAX_READ_LOOPS 60000 // How many read loops to be considered a read time-out
#define VED_MAX_READ_LINES 30	 // How many lines to read looking for a value
								 // before giving up. Also determines lines for diag dump
#define VED_BAUD_RATE 19200

// Extend this and ved_labels[] for needed inclusions
enum VE_DIRECT_DATA {
	VE_DUMP = 0,
	VE_PID,
	VE_FW,
	VE_SER,
	VE_V,
	VE_I,
	VE_VPV,
	VE_PPV,
	VE_Relay,
	VE_OR,
	VE_CS,
	VE_ERR,
	VE_LOAD,
	VE_IL,
	VE_H19,
	VE_H20,
	VE_H21,
	VE_H22,
	VE_H23,
	VE_HSDS,
	VE_MPPT,
	VE_LAST_LABEL,
};

const char ved_labels[VE_LAST_LABEL][VED_MAX_LEBEL_SIZE] PROGMEM = {
		"Dump",	 // a string that won't match any label
		"PID",     // Product ID
		"FW",      // Firmware version (16 bit)
		"SER#",    // Serial number
		"V",       // (mV) Main or channel 1 (battery) voltage
		"I",       // (mA) Main or channel 1 battery current
		"VPV",     // (mV) Panel voltage
		"PPV",     // (W) Panel power
		"Relay",   // Relay state
		"OR",      // Off reason
		"CS",      // State of operation
		"ERR",     // Error code
		"LOAD",    // Load output state (ON/OFF)
		"IL",      // (mA) Load current
		"H19",     // (0.01 kWh) Yield total (user resettable counter)
		"H20",     // (0.01 kWh) Yield today
		"H21",     // (W) Maximum power today
		"H22",     // (0.01 kWh) Yield yesterday
		"H23",     // (W) Maximum power yesterday
		"HSDS",    // Day sequence number (0..364)
		"MPPT"    // Tracker operation mode
};

class VEDirect {
public:
	VEDirect(HardwareSerial& port);
	virtual ~VEDirect();
	uint8_t begin();
	int32_t read(uint8_t target);
	void copy_raw_to_serial0(); // kept for backwards compatibility
private:
	HardwareSerial& VESerial;
};

#endif /* VEDIRECT_H_ */
