#pragma once
#include <Arduino.h>
#include "IoHwAb_PS4.h"
#include "MotionControlService.h"

class ManualModeController
{
private:
  ManualModeController();

  point data;

public:
  static ManualModeController &getInstance();

  void setup();

  void run();
};
