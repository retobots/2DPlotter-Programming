#pragma once
#include <Arduino.h>
#include <string>
#include "MotionControlService.h"
#include "IoHwAb_Servo.h"

class GcodeParserService
{
private:
  GcodeParserService();

public:
  static GcodeParserService &getInstance();

  void setup();
  void processIncomingLine(char *line, int charNB, point &actualPos);
};
