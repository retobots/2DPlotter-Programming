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
  void move(int32_t steps_x, int32_t steps_y);
  void stop();
  // Immediately set the internal current position (in steps) without moving motors.
  void setCurrentPosition(long xSteps, long ySteps);

  // --- Continuous (non-positional) control ---
  // Enable driver outputs (after stop() disables them)
  void enableOutputs();
  // Set rolling speeds in steps/second (positive=+dir, negative=-dir)
  void setRollSteps(float xStepsPerSec, float yStepsPerSec);
  // Convenience: set rolling speeds in mm/second
  void setRollMM(float xMmPerSec, float yMmPerSec);
  // Call frequently (e.g., each loop or task tick) to generate steps at set speeds
  void runSpeedTickX();
  void runSpeedTickY();
  // Stop rolling (sets speed=0 and keeps outputs enabled; call stop() to fully disable)
  void stopRolling();

  void setSpeed(float speed);
};
