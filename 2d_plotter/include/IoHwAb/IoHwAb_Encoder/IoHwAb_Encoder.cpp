/************************************************************************************************************************
 * @file    IoHwAb_Encoder.cpp
 * @author  NghiaDD
 * @date    2023-11-12
 * @brief   This file contains the implementation of the IoHwAb_Encoder class for handling rotary encoder inputs.
 * @version 1.0
 ************************************************************************************************************************/

#include "IoHwAb_Encoder.h"
#include "Pins.h"

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_Encoder::IoHwAb_Encoder()
{
}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_Encoder &IoHwAb_Encoder::getInstance()
{
  static IoHwAb_Encoder instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the rotary encoder
 ************************************************************************************************************************/
void IoHwAb_Encoder::setup()
{
  Serial.println("[SET UP]: Rotary Encoder Starting Setup!");
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);
  lastCLK = digitalRead(PIN_CLK);

  // Logging
  Serial.println("[SET UP]: Rotary Encode Done!");
}

/*************************************************************************************************************************
 * @brief Read the rotary encoder signals and update flags
 ************************************************************************************************************************/
void IoHwAb_Encoder::readEncoder()
{
  int currentCLK = digitalRead(PIN_CLK);

  if (currentCLK != lastCLK && currentCLK == LOW)
  {
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

/*************************************************************************************************************************
 * @brief Check if the encoder is being turned up
 *
 * @return true if turned up, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_Encoder::scrollUp()
{
  if (upFlag)
  {
    upFlag = false;
    return true;
  }
  return false;
}

/*************************************************************************************************************************
 * @brief Check if the encoder is being turned down
 *
 * @return true if turned down, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_Encoder::scrollDown()
{
  if (downFlag)
  {
    downFlag = false;
    return true;
  }
  return false;
}

/*************************************************************************************************************************
 * @brief Check if the rotary encoder button is pressed
 *
 * @return true if pressed, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_Encoder::isRotaryPressed()
{
  return digitalRead(PIN_SW) == LOW;
}