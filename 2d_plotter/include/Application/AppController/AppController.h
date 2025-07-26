#pragma once
#include <Arduino.h>
#include "UIMenuService.h"
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
  SD_MENU
};
class AppController
{
public:
  static AppController &getInstance();

  void setup();

private:
  AppController();

  uint8_t modeFlag = AUTOMODE;
  uint8_t workingFlag = WORKING_STATE;
  uint8_t workingStateFlag = PAUSE;
  uint8_t autoModeFlag = UGS;
  State currentState = State::MAIN_MENU;

  vector<String> fileList;

  void runMainMenu();

  void runAutoMode();

  void runManualMode();

  void runUGSMenu();

  void runSDMenu();

  void run();

  void opening();
};
