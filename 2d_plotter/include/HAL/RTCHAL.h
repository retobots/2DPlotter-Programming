#pragma once

#include <string>
#include <Arduino.h>
class RTCHAL {
public:
  static RTCHAL& getInstance();

  void setup();
  std::string getCurrentTime();

private:
  RTCHAL();
};
