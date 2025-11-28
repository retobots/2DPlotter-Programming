/*************************************************************************************************************************
 * @file    IoHwAb_Encoder.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#pragma once

#include <Arduino.h>

class IoHwAb_Encoder
{

private:
  // Constructor
  IoHwAb_Encoder();

  // Variables to track encoder state
  int lastCLK = HIGH;
  bool upFlag = false;
  bool downFlag = false;

public:
  static IoHwAb_Encoder &getInstance();
  void setup();
  void readEncoder();
  bool scrollUp();
  bool scrollDown();
  bool isRotaryPressed();
};
