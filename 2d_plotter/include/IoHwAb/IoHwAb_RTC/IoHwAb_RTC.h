#pragma once

#include <string>
#include <Arduino.h>
class IoHwAb_RTC
{
public:
  static IoHwAb_RTC &getInstance();

  void setup();
  std::string getCurrentTime();

private:
  IoHwAb_RTC();
};
