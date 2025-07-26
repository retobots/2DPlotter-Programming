#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "IoHwAb_Stepper.h"

class MotionControlService
{
private:
  MotionControlService();

  // Data lưu dữ liệu
  point Data;

public:
  static MotionControlService &getInstance();

  void setup();
  void drawLine(float xPos, float yPos);
  void stop();

  void updateData(point &newData);
};
