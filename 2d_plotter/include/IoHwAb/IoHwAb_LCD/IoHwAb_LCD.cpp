/*******************************************************************************
 * @file    IoHwAb_LCD.cpp
 * @brief   Setup thông số cho LCD. Định nghĩa các hàm hiển thị các màn hình menu
 * @version 1.0
 * @date    2025-06-19
 * @author  Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "IoHwAb_LCD.h"

/*================================================= [ DEFINITION ] ==================================================*/

LiquidCrystal_I2C lcd(0x27, 20, 4); // 0x27 là địa chỉ I2C của màn hình (có thể là 0x3F)

/*<===================================================>*/

IoHwAb_LCD::IoHwAb_LCD() {}

/*<===================================================>*/

IoHwAb_LCD &IoHwAb_LCD::getInstance()
{
  static IoHwAb_LCD instance;
  return instance;
}

/*<===================================================>*/

void IoHwAb_LCD::setup()
{
  Serial.println("[SET UP]: LCD Starting Setup!");

  // Khởi tạo LCD (I2C hoặc SPI)
  lcd.init();      // Khởi tạo LCD
  lcd.backlight(); // Bật đèn nền

  delay(500);

  // Logging
  Serial.println("[SET UP]: LCD Done!");
}

/*<===================================================>*/

void IoHwAb_LCD::clear()
{
  // Xoá màn hình
  lcd.clear();
}

/*<===================================================>*/

int IoHwAb_LCD::getMiddleXCursor(const String &text)
{
  int strLen = text.length(); // Độ dài chuỗi
  int lcdWidth = 20;          // LCD 20 cột
  int startCol = (lcdWidth - strLen) / 2;
  if (startCol < 0)
    startCol = 0; // Tránh âm nếu text quá dài
  return startCol;
}

/*<===================================================>*/

void IoHwAb_LCD::lcdDisplay(const String &text, uint8_t x, uint8_t y)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}

/*<===================================================>*/