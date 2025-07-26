#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"
#include "MotionControlService.h"
#include "FeedbackService.h"
#include <SD.h>

#define LINE_BUFFER_LENGTH 512

class AutoModeController
{

private:
  AutoModeController();

  int lineIndex = 0;
  bool lineIsComment = false;
  bool lineSemiColon = false;
  bool verbose = false;
  char line[LINE_BUFFER_LENGTH];
  char c;

  void readSerial(point &actualPoint);

  void readFile(File &file, point &actualPoint);

  // Coordinate
  point data;

  // File to read G-code commands from
  File gcodeFile;

public:
  static AutoModeController &getInstance();

  void setup();

  void getGcodeFile(const String &filename);

  void run(int choice);
};
