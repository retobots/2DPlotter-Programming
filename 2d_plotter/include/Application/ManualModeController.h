#pragma once
#include <Arduino.h>
#include "PS4ControllerHAL.h"
#include "MotionControlService.h"

class ManualModeController
{
private:
  ManualModeController();

  PS4ControllerHAL &ps4 = PS4ControllerHAL::getInstance();

  MotionControlService &MC = MotionControlService::getInstance();

  PointData paraPoint;

public:
  static ManualModeController &getInstance();

  void setup();
  void run();
};
