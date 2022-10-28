/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: ReadVEDirect.ino / ReadVEDirect.cpp
 - Provides example use of the VEDirect library

 2020.04.10 - convert to SoftwareSerial
******************************************************************/

#include "Arduino.h"
#include "VEDirect.h"
#include <LiquidCrystal.h>

// Serial pins
#define rxPin 3
#define txPin 2

// 32 bit ints to collect the data from the device
int32_t VE_soc, VE_power, VE_voltage, VE_current, VE_Vpv, VE_h20, VE_h22;
// Boolean to collect an ON/OFF value
uint8_t VE_alarm;

// VEDirect instantiated with relevant serial object
VEDirect myve(rxPin, txPin);

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;



  return btnNONE;  // when all others fail, return this...
}

void displayLCD(int Line) {  //display one line depending on line number
  switch (Line) {
      case 0:
      {
        lcd.print("Coucou Manou!     "); // print a simple message
        break;
      }
      case 1:
      {
        lcd.print("V=");
        lcd.print(VE_voltage, DEC);
        lcd.print("mV P=");
        lcd.print(VE_power, DEC);
        lcd.print("W         ");
        break;
      }
      case 2:
      {  
        lcd.print("SOC=");
        lcd.print(VE_soc, DEC);
        lcd.print(" I=");
        lcd.print(VE_current, DEC);
        lcd.print("A         ");
        break;
      }
      case 3:
      {  
        lcd.print("Vpv=");
        lcd.print(VE_Vpv, DEC);
        lcd.print("V ALRM=");
        lcd.print(VE_alarm, DEC);
        lcd.print("         ");
        break;
      }
      case 4:
      {  
        lcd.print("E=");
        lcd.print(VE_h20, DEC);
        lcd.print("Wh E-hier=");
        lcd.print(VE_h22, DEC);
        lcd.print("Wh         ");
        break;
      }
  }  
}


int currLine=0;

void setup() {
	Serial.begin(19200);		// Adjust as needed - DEBUG serial port
  myve.begin();
  lcd.begin(16, 2);              // start the library
  lcd.print(" BOOT ...");
  Serial.print("Reading values from VE.Direct");
  GetVE();
  lcd.clear();
  lcd.setCursor(0,0);
  displayLCD(currLine);
  lcd.setCursor(0,1);
  displayLCD(currLine+1);

}

void GetVE() {
	// Read the data 
		VE_soc = myve.read(VE_FW);
		VE_power = myve.read(VE_PPV);
		VE_voltage = myve.read(VE_V);
		VE_current = myve.read(VE_I);
		VE_alarm = myve.read(VE_ERR);
    VE_Vpv = myve.read(VE_VPV);
    VE_h20 = myve.read(VE_H20); //(0.01 kWh) Yield today
    VE_h22 = myve.read(VE_H22);// (0.01 kWh) Yield yesterday
}

void loop() {
  //Serial.println(currLine);

  lcd_key = read_LCD_buttons();

  switch (lcd_key) // depending on which button was pushed, we perform an action
  {
    case btnUP:
    {
      currLine= (currLine + 1) % 4;
      delay(500);
      break;
    }
    case btnDOWN:
    {
      currLine= (currLine - 1) % 4;
      currLine= currLine * (currLine>0);
      delay(500);
      break;
    }
    case btnLEFT:
    {
      GetVE();
      // Print each of the values
      Serial.print("State of Charge (SOC): ");
      Serial.println(VE_soc, DEC);
      Serial.print("Power:                 ");
      Serial.println(VE_power, DEC);
      Serial.print("Voltage                ");
      Serial.println(VE_voltage, DEC);
      Serial.print("Current                ");
      Serial.println(VE_current, DEC);
      Serial.print("Alarm                  ");
      Serial.println(VE_alarm, DEC);
      Serial.println();
      break;
    }
    case btnSELECT:
    {
      // Copy the raw data stream (minus the \r) to Serial0
      // Call read() with a token that won't match any VE.Direct labels
      Serial.println("All data from device:");
      myve.read(VE_DUMP);
      delay(500);
      break;
    }
  }
  //redraw screen
  //GetVE();
  lcd.setCursor(0,0);
  displayLCD(currLine);
  lcd.setCursor(0,1);
  displayLCD(currLine+1);
}