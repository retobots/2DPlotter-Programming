/*************************************************************************************************************************
 * @file    UIMenuService.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#pragma once

#include <Arduino.h>
#include "IoHwAb_Buzzer.h"
#include "IoHwAb_LCD.h"
#include "IoHwAb_Encoder.h"
#include "IoHwAb_Button.h"
#include "IoHwAb_PS4.h"
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

  void PS4ModeScreen(int choice);
};
