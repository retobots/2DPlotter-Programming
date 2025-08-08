#pragma once

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
