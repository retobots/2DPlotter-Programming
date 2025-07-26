#pragma once

/**
 * @file    IoHwAb_Encoder.h
 * @author  Do Duc Nghia
 * @brief   File này chứa các hàm đọc tín hiệu rotary encoder, setup thông số encoder
 * @version 1.0
 * @date    2025-06-19
 */

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include <Arduino.h>

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   IoHwAb_Encoder
 * @brief   Class chứa các hàm đọc tín hiệu của rotary encoder, setup cho rotary encoder
 ****************************************************************************************
 */

class IoHwAb_Encoder
{

private:
  /**
   * @brief Constructo khởi tạo các thông số ban đầu khi khởi tạo đối tượng
   *
   */
  IoHwAb_Encoder();

  /*======================== [ Các biến flag ] ===========================*/
  int lastCLK = HIGH;
  bool upFlag = false;
  bool downFlag = false;

public:
  /**
   * @brief   khởi tạo đối tượng
   *
   * @return  IoHwAb_Encoder&
   */
  static IoHwAb_Encoder &getInstance();

  /**
   * @brief setup thông số
   *
   */
  void setup();

  /**
   * @brief đọc tín hiệu encoder
   *
   */
  void readEncoder();

  /**
   * @brief  kiểm tra đang quay lên hay không
   *
   * @return true
   * @return false
   */
  bool scrollUp();

  /**
   * @brief  kiểm tra đang quay xuống hay không
   *
   * @return true
   * @return false
   */
  bool scrollDown();

  /**
   * @brief  kiểm tra có nhấn nút trên rotary encoder hay không
   *
   * @return true
   * @return false
   */
  bool isRotaryPressed();
};
