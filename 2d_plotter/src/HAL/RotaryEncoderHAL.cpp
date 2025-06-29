/*******************************************************************************
 * @file    RotaryEncoderHAL.cpp
 * @brief   Định nghĩa các hàm trong file header
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include "HAL/RotaryEncoderHAL.h"
#include "Pins.h"

/*================================================= [ DEFINITION ] ==================================================*/

/*<===================================================>*/

RotaryEncoderHAL::RotaryEncoderHAL()
{
  setup();
}

/*<===================================================>*/

RotaryEncoderHAL &RotaryEncoderHAL::getInstance()
{
  Serial.println("INSTANCE ROTARY CREATED");
  static RotaryEncoderHAL instance;
  return instance;
}

/*<===================================================>*/

void RotaryEncoderHAL::setup()
{
  // Cấu hình encoder
  pinMode(PIN_CLK, INPUT);
  pinMode(PIN_DT, INPUT);
  pinMode(PIN_SW, INPUT_PULLUP);
  lastCLK = digitalRead(PIN_CLK);
}

/*<===================================================>*/

void RotaryEncoderHAL::readEncoder()
{
  int currentCLK = digitalRead(PIN_CLK);

  if (currentCLK != lastCLK && currentCLK == LOW)
  { // Sườn xuống
    int dtState = digitalRead(PIN_DT);

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

bool RotaryEncoderHAL::scrollUp()
{
  if (upFlag)
  {
    upFlag = false;
    return true;
  }
  return false;
}

/*<===================================================>*/

bool RotaryEncoderHAL::scrollDown()
{
  if (downFlag)
  {
    downFlag = false;
    return true;
  }
  return false;
}

/*<===================================================>*/

bool RotaryEncoderHAL::isRotaryPressed()
{
  return digitalRead(PIN_SW) == LOW;
}