#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "StepperHAL.h"

class MotionControlService
{
private:
  MotionControlService();

  // Khai báo stepper
  StepperHAL &stepper;

  // Data lưu dữ liệu
  point Data;

public:
  static MotionControlService &getInstance();

  void setup();
  void drawLine(float xPos, float yPos);
  void stop();

  void updateData(point &newData);
};
