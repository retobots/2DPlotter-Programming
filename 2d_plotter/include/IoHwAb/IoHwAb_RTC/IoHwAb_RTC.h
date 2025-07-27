#pragma once

#include <string>
#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>
#include "Pins.h"

class IoHwAb_RTC
{
private:
  IoHwAb_RTC();

  DS3231 rtc;
  Time startTime;
  bool isStart = false;

public:
  static IoHwAb_RTC &getInstance();

  void setup();
  void startTimer();
  String getElapsedTime();
  void changeStatus();
};
