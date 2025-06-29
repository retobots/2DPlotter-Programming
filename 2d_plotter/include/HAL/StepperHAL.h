#pragma once
#include <Arduino.h>
class StepperHAL {
public:
  static StepperHAL& getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();

private:
  StepperHAL();
};
