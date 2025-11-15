#pragma once
#include <Arduino.h>
#include "GcodeParserService.h"
#include "MotionControlService.h"
#include "FeedbackService.h"
#include "UIMenuService.h"
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
  int lineVectorIndex = 0;
  int charVectorIndex = 0;
  bool isLineFull = false;

  point data = {0.0, 0.0};
  File gcodeFile;
  bool active = true;
  bool paused = false;
  bool cancelled = false;
  int percentage = 0;
  String time = "";
  int statusFlag = 0;

  bool isInitialized = false;

  void readSerial(point &actualPoint);

  void readFile(File &file, point &actualPoint, int workingFlag, int &percentage, String &time, int &statusFlag);

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

  int getPercentage() const;

  String getTime() const;

  int getStatusFlag() const;
};
