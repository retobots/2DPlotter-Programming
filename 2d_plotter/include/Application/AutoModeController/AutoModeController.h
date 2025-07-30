#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"
#include "MotionControlService.h"
#include "FeedbackService.h"
#include "UIMenuService.h"
#include "IoHwAb_RTC.h"
#include "IoHwAb_SD.h"
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

  point data;
  File gcodeFile;
  bool active = false;
  bool paused = false;
  bool cancelled = false;

  void readSerial(point &actualPoint);

  void readFile(File &file, point &actualPoint, int workingFlag);

public:
  static AutoModeController &getInstance();

  void setup();

  void getGcodeFile(const String &filename);

  void pauseSD();

  void continueSD();

  void cancelSD();

  void runSD(int workingFlag);

  void resetSD();

  void resetSerial();

  void runSerial();
};
