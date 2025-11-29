/************************************************************************************************************************
 * @file     GcodeParserService.h
 * @brief    Declarations of functions in the header file
 * @author   Do Duc Nghia
 * @version  1.0
 ************************************************************************************************************************/

#pragma once
#include <Arduino.h>
#include <string>
#include "MotionControlService.h"
#include "IoHwAb_Servo.h"
#include <esp_system.h>

class GcodeParserService
{
private:
  GcodeParserService();

public:
  static GcodeParserService &getInstance();

  void setup();
  void processIncomingLine(char *line, int charNB, point &actualPos);
};
