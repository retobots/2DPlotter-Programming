#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"

class AutoModeController
{

private:
  AutoModeController();
  GcodeParserService GPS;

public:
  static AutoModeController &getInstance();

  void setup();
  void readSerial(point &actualPoint);
};
