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
  // Simple digital I/O setup - no PWM conflicts
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW); // Start with buzzer off

  // Logging
  Serial.println("[SET UP]: Buzzer Done!");
}

/*<===================================================>*/

void IoHwAb_Buzzer::buzzerTone(int freq)
{
  // Generate tone using digitalWrite with calculated timing
  if (freq > 0)
  {
    Serial.print("[BUZZER]: Playing tone at ");
    Serial.print(freq);
    Serial.println(" Hz");

    // Calculate half period in microseconds
    unsigned long halfPeriod = 1000000 / (freq * 2);

    // Play tone for a short duration (100ms)
    unsigned long startTime = millis();
    while (millis() - startTime < 100)
    {
      digitalWrite(PIN_BUZZER, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(PIN_BUZZER, LOW);
      delayMicroseconds(halfPeriod);
    }
  }
}

/*<===================================================>*/

void IoHwAb_Buzzer::buzzerStop()
{
  digitalWrite(PIN_BUZZER, LOW);
  Serial.println("[BUZZER]: Stopped");
}

/*<===================================================>*/

void IoHwAb_Buzzer::beepOnce()
{
  // Logging
  Serial.println("[BUZZER]: Beep!");

  buzzerTone(2000); // Tạo sóng âm 2kHz trong 100ms
  delay(100);       // Small pause
  buzzerStop();     // Tắt buzzer
}

/*<===================================================>*/

void IoHwAb_Buzzer::startingSoundBuzzer()
{
  Serial.println("[BUZZER]: Playing startup melody");

  int melody[] = {523, 659, 784, 1047}; // C5, E5, G5, C6
  int duration[] = {200, 200, 300, 400};

  for (int i = 0; i < 4; i++)
  {
    buzzerTone(melody[i]);    // This already includes the tone duration (100ms)
    delay(duration[i] - 100); // Additional delay minus the tone duration
  }
  buzzerStop();
}

/*<===================================================>*/