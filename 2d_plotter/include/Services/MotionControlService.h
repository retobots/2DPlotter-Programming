#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "StepperHAL.h"

typedef struct point
{
  float x;
  float y;
} point;

// extern point actualPoint;

class MotionControlService
{
private:
  MotionControlService();

  // Khai báo stepper
  StepperHAL &stepper = StepperHAL::getInstance();

  // Vị trí hiện tại (mm)
  float Xpos = X_MIN;
  float Ypos = Y_MIN;

public:
  static MotionControlService &getInstance();

  void setup();
  void drawLine(float xPos, float yPos);
  void stop();
};
