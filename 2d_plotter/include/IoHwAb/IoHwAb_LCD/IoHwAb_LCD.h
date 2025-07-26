#pragma once

/*******************************************************************************
 * @file    IoHwAb_LCD.h
 * @brief   Setup thông số cho LCD. Khai báo các hàm hiển thị các màn hình menu
 * @version 1.0
 * @date    2025-06-19
 * @author  Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include <string>
#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "Pins.h"
#include "IoHwAb_SD.h"
#include <vector>
using namespace std;

/*================================================ [ CONFIGURATION ] ==================================================*/

/*<Khai báo lcd>*/
extern LiquidCrystal_I2C lcd;

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   IoHwAb_LCD
 * @brief   Class chứa các chức năng hiển thị của màn hình lcd
 * @details Cấu trúc này chứa thông số đối tượng lcd, hàm hiển thị màn hình chính, status
 ****************************************************************************************
 */

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
