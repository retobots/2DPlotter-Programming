#pragma once
/*************************************************************************************************************************
 * @file    IoHwAb_Buzzer.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include <Arduino.h>

class IoHwAb_Buzzer
{

private:
  // Constructor
  IoHwAb_Buzzer();

public:
  static IoHwAb_Buzzer &getInstance();
  void setup();
  void buzzerTone(int freq);
  void buzzerStop();
  void beepOnce();
  void startingSoundBuzzer();
};
