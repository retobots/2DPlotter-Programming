#pragma once

/**
 * @file    RotaryEncoderHAL.h
 * @author  Do Duc Nghia
 * @brief   File này chứa các hàm đọc tín hiệu rotary encoder, setup thông số encoder
 * @version 1.0
 * @date    2025-06-19
 */

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include <Arduino.h>

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   RotaryEncoderHAL
 * @brief   Class chứa các hàm đọc tín hiệu của rotary encoder, setup cho rotary encoder
 ****************************************************************************************
 */

class RotaryEncoderHAL
{

private:
  /**
   * @brief Constructo khởi tạo các thông số ban đầu khi khởi tạo đối tượng
   *
   */
  RotaryEncoderHAL();

  /*======================== [ Các biến flag ] ===========================*/
  int lastCLK = HIGH;
  bool upFlag = false;
  bool downFlag = false;

public:
  /**
   * @brief   khởi tạo đối tượng
   *
   * @return  RotaryEncoderHAL&
   */
  static RotaryEncoderHAL &getInstance();

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
