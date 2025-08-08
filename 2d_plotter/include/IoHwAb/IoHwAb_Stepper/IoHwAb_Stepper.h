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

  // --- Continuous (non-positional) control ---
  // Enable driver outputs (after stop() disables them)
  void enableOutputs();
  // Set rolling speeds in steps/second (positive=+dir, negative=-dir)
  void setRollSteps(float xStepsPerSec, float yStepsPerSec);
  // Convenience: set rolling speeds in mm/second
  void setRollMM(float xMmPerSec, float yMmPerSec);
  // Call frequently (e.g., each loop or task tick) to generate steps at set speeds
  void runSpeedTick();
  // Stop rolling (sets speed=0 and keeps outputs enabled; call stop() to fully disable)
  void stopRolling();

  void setSpeed(float speed);
};
