#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"

class AutoModeController
{

private:
  AutoModeController();
  GcodeParserService &GPS = GcodeParserService::getInstance();

  int lineIndex = 0;
  bool lineIsComment = false;
  bool lineSemiColon = false;
  bool verbose = false;
  char line[LINE_BUFFER_LENGTH];
  char c;

public:
  static AutoModeController &getInstance();

  void setup();
  void readSerial(point &actualPoint);
};
