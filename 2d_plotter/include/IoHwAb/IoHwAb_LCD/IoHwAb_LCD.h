#pragma once
/*************************************************************************************************************************
 * @file    IoHwAb_LCD.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include <string>
#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "Pins.h"
#include "IoHwAb_SD.h"
#include <vector>
using namespace std;

// Declare the LCD object (I2C or SPI)
extern LiquidCrystal_I2C lcd;

class IoHwAb_LCD
{
private:
  IoHwAb_LCD();

public:
  static IoHwAb_LCD &getInstance();

  void setup();

  int getMiddleXCursor(const String &text);

  void lcdDisplay(const String &text, uint8_t x, uint8_t y);

  void clear();
};
