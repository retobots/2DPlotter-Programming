#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"
#include "MotionControlService.h"

#define LINE_BUFFER_LENGTH 512

class AutoModeController
{

private:
  AutoModeController();

  GcodeParserService &GPS;
  MotionControlService &MC;

  int lineIndex = 0;
  bool lineIsComment = false;
  bool lineSemiColon = false;
  bool verbose = false;
  char line[LINE_BUFFER_LENGTH];
  char c;

  void readSerial(point &actualPoint);

  // Coordinate
  point data;

public:
  static AutoModeController &getInstance();

  void setup();

  void run();
};
