/*************************************************************************************************************************
 * @file    IoHwAb_LCD.cpp
 * @author  Do Duc Nghia
 * @version 1.0
 * @date    2025-06-19
 * @brief   Definitions of functions in the header file
 ************************************************************************************************************************/

#include "IoHwAb_LCD.h"

// Initialize the LCD object (I2C or SPI)
LiquidCrystal_I2C lcd(0x27, 20, 4);

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_LCD::IoHwAb_LCD() {}

/*************************************************************************************************************************
 * @brief Get the singleton instance of IoHwAb_LCD
 ************************************************************************************************************************/
IoHwAb_LCD &IoHwAb_LCD::getInstance()
{
  static IoHwAb_LCD instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the LCD
 ************************************************************************************************************************/
void IoHwAb_LCD::setup()
{
  Serial.println("[SET UP]: LCD Starting Setup!");

  lcd.init();
  lcd.backlight();

  delay(500);

  // Logging
  Serial.println("[SET UP]: LCD Done!");
}

/*************************************************************************************************************************
 * @brief Clear the LCD screen
 ************************************************************************************************************************/
void IoHwAb_LCD::clear()
{
  // Xoá màn hình
  lcd.clear();
}

/*************************************************************************************************************************
 * @brief Get the middle X cursor position for centering text on the LCD
 ************************************************************************************************************************/
int IoHwAb_LCD::getMiddleXCursor(const String &text)
{
  int strLen = text.length(); // Length of the text
  int lcdWidth = 20;          // Width of the LCD in characters
  int startCol = (lcdWidth - strLen) / 2;
  if (startCol < 0)
    startCol = 0;
  return startCol;
}

/*************************************************************************************************************************
 * @brief Display text on the LCD at specified coordinates
 ************************************************************************************************************************/
void IoHwAb_LCD::lcdDisplay(const String &text, uint8_t x, uint8_t y)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}