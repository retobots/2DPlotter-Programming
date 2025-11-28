/*************************************************************************************************************************
 * @file    IoHwAb_Buzzer.cpp
 * @brief   Definitions of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#include "IoHwAb_Buzzer.h"
#include "Pins.h"

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_Buzzer::IoHwAb_Buzzer() {}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_Buzzer &IoHwAb_Buzzer::getInstance()
{
  static IoHwAb_Buzzer instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the buzzer
 ************************************************************************************************************************/
void IoHwAb_Buzzer::setup()
{
  Serial.println("[SET UP]: Buzzer Starting Setup!");
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  Serial.println("[SET UP]: Buzzer Done!");
}

/*************************************************************************************************************************
 * @brief Generate a tone on the buzzer
 *
 * @param freq Frequency of the tone in Hz
 ************************************************************************************************************************/
void IoHwAb_Buzzer::buzzerTone(int freq)
{
  if (freq > 0)
  {
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

/*************************************************************************************************************************
 * @brief Stop the buzzer
 ************************************************************************************************************************/
void IoHwAb_Buzzer::buzzerStop()
{
  digitalWrite(PIN_BUZZER, LOW);
  Serial.println("[BUZZER]: Stopped");
}

/*************************************************************************************************************************
 * @brief Beep the buzzer once
 ************************************************************************************************************************/
void IoHwAb_Buzzer::beepOnce()
{
  buzzerTone(2000); // Create a 2000 Hz tone
  delay(100);       // Small pause
  buzzerStop();     // Stop the buzzer
}

/*************************************************************************************************************************
 * @brief Play the startup sound melody on the buzzer
 ************************************************************************************************************************/
void IoHwAb_Buzzer::startingSoundBuzzer()
{
  int melody[] = {523, 659, 784, 1047}; // C5, E5, G5, C6
  int duration[] = {200, 200, 300, 400};

  for (int i = 0; i < 4; i++)
  {
    buzzerTone(melody[i]);    // This already includes the tone duration (100ms)
    delay(duration[i] - 100); // Additional delay minus the tone duration
  }
  buzzerStop();
}