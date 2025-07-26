#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "Pins.h"
#include "Settings.h"

class StepperHAL
{
private:
  StepperHAL();

  AccelStepper *stepperX;
  AccelStepper *stepperY;

public:
  static StepperHAL &getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();
};
