/*******************************************************************************
 * @file    IoHwAb_Encoder.cpp
 * @brief   Định nghĩa các hàm trong file header
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include "IoHwAb_Encoder.h"
#include "Pins.h"

/*================================================= [ DEFINITION ] ==================================================*/

/*<===================================================>*/

IoHwAb_Encoder::IoHwAb_Encoder()
{
}

/*<===================================================>*/

IoHwAb_Encoder &IoHwAb_Encoder::getInstance()
{
  static IoHwAb_Encoder instance;
  return instance;
}

/*<===================================================>*/

void IoHwAb_Encoder::setup()
{
  Serial.println("[SET UP]: Rotary Encoder Starting Setup!");
  // Cấu hình encoder
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);
  lastCLK = digitalRead(PIN_CLK);

  // Logging
  Serial.println("[SET UP]: Rotary Encode Done!");
}

/*<===================================================>*/

void IoHwAb_Encoder::readEncoder()
{
  int currentCLK = digitalRead(PIN_CLK);

  // Serial.print("Encoder CLK state: ");
  // Serial.println(currentCLK);

  if (currentCLK != lastCLK && currentCLK == LOW)
  { // Sườn xuống
    int dtState = digitalRead(PIN_DT);

    // Serial.print("Encoder DT state: ");
    // Serial.println(dtState);

    if (dtState != currentCLK)
    {
      upFlag = true;
    }
    else
    {
      downFlag = true;
    }
  }

  lastCLK = currentCLK;
}

/*<===================================================>*/

bool IoHwAb_Encoder::scrollUp()
{
  if (upFlag)
  {
    upFlag = false;
    return true;
  }
  return false;
}

/*<===================================================>*/

bool IoHwAb_Encoder::scrollDown()
{
  if (downFlag)
  {
    downFlag = false;
    return true;
  }
  return false;
}

/*<===================================================>*/

bool IoHwAb_Encoder::isRotaryPressed()
{
  return digitalRead(PIN_SW) == LOW;
}