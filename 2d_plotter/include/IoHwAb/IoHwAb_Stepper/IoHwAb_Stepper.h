#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "Pins.h"
#include "Settings.h"

class IoHwAb_Stepper
{
private:
  IoHwAb_Stepper();

  AccelStepper *stepperX;
  AccelStepper *stepperY;

public:
  static IoHwAb_Stepper &getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();
};
