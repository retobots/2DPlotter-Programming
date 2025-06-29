#pragma once

/*******************************************************************************
 * @file    LcdHAL.h
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
#include "SDCardHAL.h"

/*================================================ [ CONFIGURATION ] ==================================================*/

/*<Khai báo lcd>*/
extern LiquidCrystal_I2C lcd;

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   LcdHAL
 * @brief   Class chứa các chức năng hiển thị của màn hình lcd
 * @details Cấu trúc này chứa thông số đối tượng lcd, hàm hiển thị màn hình chính, status
 ****************************************************************************************
 */

class LcdHAL
{
private:
  LcdHAL();

public:
  /**
   * @brief   Khởi tạo đối tượng
   * @details Hàm này sẽ giúp khởi tạo đối tượng từ class, đối tượng sẽ chứa các thông số,
   *          phương thức đã được định nghĩa và khai báo sẵn phục vụ cho quá trình lập trình
   * @return  LcdHAL
   */
  static LcdHAL &getInstance();

  /**
   * @brief   Setup LCD
   * @details Hàm này sẽ giúp set các pins, thông số cho LCD
   */
  void setup();

  /**
   * @brief   Hiển thị màn hình chào mừng
   * @details Hàm này sẽ giúp hiển thị màn hình bắt đầu khi khởi động máy
   */
  void welcomeScreen();

  /**
   * @brief   Hiển thị màn hình chọn mode
   * @details Hàm này sẽ hiển thị các lựa chọn mode cho người dùng
   */
  void modeScreen(uint8_t update);

  /**
   * @brief   Lấy vị trí căn giữa của chữ
   * @details Hàm này giúp tính toán đưa ra vị trí căn giữa của chữ trên màn hình LCD
   *
   * @param   text chữ dùng để hiển thị
   * @return  * int
   */
  int getMiddleXCursor(const String &text);

  /**
   * @brief Hiển thị chữ với tọa độ tùy ý
   *
   * @param text  Chữ muốn hiển thị
   * @param x     Tọa độ x trên màn hình LCD
   * @param y     Tọa độ y trên màn hình LCD
   */
  void lcdDisplay(const String &text, uint8_t x, uint8_t y);

  /**
   * @brief Hàm hiển thị các lựa chọn trong automode
   *
   * @param update biến update giúp cập nhật màn hình khi nhận tín hiệu nút nhấn
   */
  void automodeScreen(uint8_t update);

  /**
   * @brief
   *
   * @param files       Mảng chứa tên của các file đọc được
   * @param signal      Tín hiệu xử lý của encoder
   * @param ar_idx      Vị trí mũi tên
   * @param startIndex  Số thứ tự file ứng với hàng đầu tiên
   * @param fileCount   Số lượng file
   * @param state       Mode hiện tại
   */
  void sdModeScreen(String files[], int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state);

  /**
   * @brief Hàm hiển thị màn hình thông số, status khi chọn serial Mode
   *
   * @param file              tên file
   * @param percenum          phần trăm công việc
   * @param workingStateMode  biến cập nhật màn hình
   */
  void serialModeScreen(String file, int percenum, uint8_t workingStateMode);

  /**
   * @brief Màn hình loading
   *
   */
  void loadingScreen();

  /**
   * @brief xóa màn hình
   *
   */
  void clear();
};