#pragma once
#include <Arduino.h>
#include <string>
#include "MotionControlService.h"
#include "ServoHAL.h"

#define LINE_BUFFER_LENGTH 512

class GcodeParserService
{
private:
  GcodeParserService();

public:
  static GcodeParserService &getInstance();

  void setup();
  void processIncomingLine(char *line, int charNB, point &actualPos);
};
