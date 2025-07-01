#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "Pins.h"
#include "Settings.h"

class StepperHAL
{
public:
  static StepperHAL &getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();

private:
  StepperHAL();
};
