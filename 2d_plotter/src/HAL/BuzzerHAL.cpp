/*******************************************************************************
 * @file    BuzzerHAL.cpp
 * @brief   Định nghĩa các hàm trong file header
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ******************************************************************************/
/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "HAL/BuzzerHAL.h"
#include "Pins.h"

/*================================================= [ DEFINITION ] ==================================================*/

BuzzerHAL::BuzzerHAL() { setup(); }

/*<===================================================>*/

BuzzerHAL &BuzzerHAL::getInstance()
{
  Serial.println("INSTANCE BUZZER CREATED");
  static BuzzerHAL instance;
  return instance;
}

/*<===================================================>*/

void BuzzerHAL::setup()
{
  // Logging
  Serial.println("Set Up Buzzer!");

  // Setup
  ledcSetup(BUZZER_CHANNEL, 2000, 8); // tần số 2kHz, độ phân giải 8-bit
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
}

/*<===================================================>*/

void BuzzerHAL::buzzerTone(int freq)
{
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, freq);
}

/*<===================================================>*/

void BuzzerHAL::buzzerStop()
{
  ledcWriteTone(BUZZER_CHANNEL, 0);
  ledcDetachPin(PIN_BUZZER);
}

/*<===================================================>*/

void BuzzerHAL::beepOnce()
{
  // Logging
  Serial.println("Beep!");

  buzzerTone(2000); // Tạo sóng âm 2kHz
  delay(300);       // Kêu 300ms
  buzzerStop();     // Tắt buzzer
}

/*<===================================================>*/

void BuzzerHAL::startingSoundBuzzer()
{
  // Logging
  Serial.println("Buzzer khởi động - Played!");

  int melody[] = {523, 659, 784, 1047}; // C5, E5, G5, C6
  int duration[] = {200, 200, 300, 400};

  for (int i = 0; i < 4; i++)
  {
    buzzerTone(melody[i]);
    delay(duration[i]);
  }
  buzzerStop();
}

/*<===================================================>*/