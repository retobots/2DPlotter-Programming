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
#include "IoHwAb_Buzzer.h"
#include "IoHwAb_LCD.h"
#include "IoHwAb_Encoder.h"
#include "IoHwAb_Button.h"
#include <vector>
#include <string>
using namespace std;

class UIMenuService
{
private:
  UIMenuService();

public:
  static UIMenuService &getInstance();

  void setup();

  void run();

  void welcomeScreen();

  void modeScreen(uint8_t update);

  void automodeScreen(uint8_t update);

  void sdModeScreen(vector<String> files, int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state, String &filename);

  void serialModeScreen(uint8_t workingStateMode);

  void loadingScreen();

  void statusScreen(String filename, int percenum, String time, uint8_t workingStateMode, uint8_t workingStateFlag);

  void UGSModeScreen();
};
