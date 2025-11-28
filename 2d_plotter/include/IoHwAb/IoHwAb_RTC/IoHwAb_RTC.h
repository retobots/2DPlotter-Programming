#pragma once

/*************************************************************************************************************************
 * @file    IoHwAb_RTC.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include <string>
#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include "Pins.h"

class IoHwAb_RTC
{
private:
  IoHwAb_RTC();

  RTC_DS3231 rtc;
  DateTime startTime;
  bool isStart = false;

public:
  static IoHwAb_RTC &getInstance();

  void setup();
  void startTimer();
  String getElapsedTime();
  void changeStatus();
  void resetTimer();
};
