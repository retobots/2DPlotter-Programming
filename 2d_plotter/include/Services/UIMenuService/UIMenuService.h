#pragma once

/**
 * @file    UIMenuService.h
 * @author  Do Duc Nghia
 * @brief   File này quản lý class, thông số cho Menu
 * @version 1.0
 * @date    2025-06-20
 */

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include <Arduino.h>
#include "BuzzerHAL.h"
#include "LcdHAL.h"
#include "RotaryEncoderHAL.h"
#include "ButtonHAL.h"
#include <vector>
#include <string>
using namespace std;

/*================================================ [ CONFIGURATION ] ==================================================*/

#define AUTOMODE 0
#define MANUALMODE 1

#define UGS 0
#define SD 1

#define WORKING_STATE 0
#define CANCEL 1

#define PAUSE 0
#define CONTINUE 1

/**
 * @brief enum chứa các mode của menu
 *
 */
enum State
{
  MAIN_MENU,
  AUTO_MODE,
  MANUAL_MODE,
  UGS_MENU,
  SD_MENU
};

/**
 * @brief class UIMenu giúp quản lý các phương thức
 *
 */
class UIMenuService
{
private:
  UIMenuService();

  uint8_t modeFlag = AUTOMODE;
  uint8_t workingFlag = WORKING_STATE;
  uint8_t workingStateFlag = PAUSE;
  uint8_t autoModeFlag = UGS;
  State currentState = MAIN_MENU;

  void runMainMenu();

  void runAutoMode();

  void runManualMode();

  void runUGSMenu();

  void runSDMenu();

  // File List
  vector<String> fileList;

public:
  /**
   * @brief   Khởi tạo đối tượng menu
   *
   * @return  UIMenuService&
   */
  static UIMenuService &getInstance();

  /**
   * @brief Hàm setup UIMenu và các đối tượng khác
   *
   */
  void setup();

  // bool cancelSignal();

  /**
   * @brief Hàm chạy chương trình điều phối logic Menu
   *
   */
  void run();

  void opening();
};
