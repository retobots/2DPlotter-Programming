#pragma once

/**
 * @file    IoHwAb_Buzzer.h
 * @author  Do Duc Nghia
 * @brief   File này chứa các khai báo, thông số, hàm điều khiển buzzer
 * @version 1.0
 * @date    2025-06-19
 */
/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include <Arduino.h>

/*================================================ [ CONFIGURATION ] ==================================================*/

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   IoHwAb_Buzzer
 * @brief   Class chứa các chức năng điều khiển buzzer phát âm thanh
 * @details Cấu trúc này chứa thông số đối tượng Buzzer, hàm phát âm thanh, setup thông số
 ****************************************************************************************
 */
class IoHwAb_Buzzer
{

private:
  /**
   * @brief Constructor khởi tạo thông số cho buzzer khi khởi tạo đối tượng
   *
   */
  IoHwAb_Buzzer();

public:
  /**
   * @brief Tạo đối tượng buzzer
   *
   * @return IoHwAb_Buzzer&
   */
  static IoHwAb_Buzzer &getInstance();

  /**
   * @brief setup cho buzzer
   *
   */
  void setup();

  /**
   * @brief Điều chỉnh tone phát âm thanh của buzzer
   *
   * @param freq tần số phát âm thanh
   */
  void buzzerTone(int freq);

  /**
   * @brief ngưng phát âm thanh
   *
   */
  void buzzerStop();

  /**
   * @brief phát âm thanh 1 lần
   *
   */
  void beepOnce();

  /**
   * @brief phát đoạn âm thanh mở đầu khi mở máy
   *
   */
  void startingSoundBuzzer();
};
