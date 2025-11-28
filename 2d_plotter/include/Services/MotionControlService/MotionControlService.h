/************************************************************************************************************************
 * @file     MotionControlService.h
 * @brief    Declarations of functions in the header file
 * @author   Do Duc Nghia
 * @version  1.0
 ************************************************************************************************************************/
#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "IoHwAb_Stepper.h"
#include "IoHwAb_Servo.h"

class MotionControlService
{
private:
  MotionControlService();

  point Data = {0.0, 0.0};

public:
  static MotionControlService &getInstance();

  void setup();
  void drawLine(float xPos, float yPos);
  void moveTo(float xPos, float yPos);
  void stop();
  void drawArc(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset, bool is_clockwise = true);
  void updateData(point &newData);
};