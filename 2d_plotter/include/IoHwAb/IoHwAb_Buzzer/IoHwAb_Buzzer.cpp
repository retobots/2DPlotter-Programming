/*******************************************************************************
 * @file    IoHwAb_Buzzer.cpp
 * @brief   Định nghĩa các hàm trong file header
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ******************************************************************************/
/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "IoHwAb_Buzzer.h"
#include "Pins.h"

/*================================================= [ DEFINITION ] ==================================================*/

IoHwAb_Buzzer::IoHwAb_Buzzer() {}

/*<===================================================>*/

IoHwAb_Buzzer &IoHwAb_Buzzer::getInstance()
{
  static IoHwAb_Buzzer instance;
  return instance;
}

/*<===================================================>*/

void IoHwAb_Buzzer::setup()
{
  Serial.println("[SET UP]: Buzzer Starting Setup!");
  // Setup
  ledcSetup(BUZZER_CHANNEL, 2000, 8); // tần số 2kHz, độ phân giải 8-bit
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);

  // Logging
  Serial.println("[SET UP]: Buzzer Done!");
}

/*<===================================================>*/

void IoHwAb_Buzzer::buzzerTone(int freq)
{
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, freq);
}

/*<===================================================>*/

void IoHwAb_Buzzer::buzzerStop()
{
  ledcWriteTone(BUZZER_CHANNEL, 0);
  ledcDetachPin(PIN_BUZZER);
}

/*<===================================================>*/

void IoHwAb_Buzzer::beepOnce()
{
  // Logging
  Serial.println("Beep!");

  buzzerTone(2000); // Tạo sóng âm 2kHz
  delay(300);       // Kêu 300ms
  buzzerStop();     // Tắt buzzer
}

/*<===================================================>*/

void IoHwAb_Buzzer::startingSoundBuzzer()
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