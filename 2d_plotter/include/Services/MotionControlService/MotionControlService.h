#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "IoHwAb_Stepper.h"
#include "IoHwAb_Servo.h"

class MotionControlService
{
private:
  MotionControlService();

  // Data lưu dữ liệu
  point Data = {0.0, 0.0};

public:
  static MotionControlService &getInstance();

  void setup();
  void drawLine(float xPos, float yPos);
  void moveTo(float xPos, float yPos);
  void stop();
  void drawArcCW(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset);
  void drawArcCCW(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset);
  void updateData(point &newData);
};