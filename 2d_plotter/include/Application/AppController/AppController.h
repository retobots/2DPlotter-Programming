#pragma once
#include <Arduino.h>
#include "UIMenuService.h"
#include "FeedbackService.h"
#include "AutoModeController.h"
#include "ManualModeController.h"
#include "IoHwAb_RTC.h"
#include "IoHwAb_SD.h"
using namespace std;

#define AUTOMODE 0
#define MANUALMODE 1

#define UGS 0
#define SD 1

#define WORKING_STATE 0
#define CANCEL 1

#define PAUSE 0
#define CONTINUE 1

enum State
{
  MAIN_MENU,
  AUTO_MODE,
  MANUAL_MODE,
  UGS_MENU,
  SD_MENU,
  SD_STATUS,
  SERIAL_MODE
};
class AppController
{
public:
  static AppController &getInstance();

  void setup();

  void run();

  uint8_t modeFlag = AUTOMODE;
  uint8_t workingFlag = WORKING_STATE;
  uint8_t workingStateFlag = PAUSE;
  uint8_t autoModeFlag = UGS;
  State currentState = State::MAIN_MENU;

  // Variable to save the selected file name
  String selectedFile;

  // Task handles for proper cleanup
  TaskHandle_t taskRunSDHandle = NULL;
  TaskHandle_t taskUIHandle = NULL;

private:
  AppController();

  // Vector to hold file names
  vector<String> fileList;

  void runMainMenu();

  void runAutoMode();

  void runManualMode();

  void runUGSMenu();

  void runSDMenu();

  void opening();

  void runSDMode();
};

void taskRunSD(void *pvParameters);

void taskUI(void *pvParameters);
