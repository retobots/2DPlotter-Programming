/**
 * @file    UIMenuService.cpp
 * @author  Do Duc Nghia
 * @brief   File chứa logic chính của UIMenu
 * @version 1.0
 * @date    2025-06-20
 */

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/

#include "UIMenuService.h"

/*================================================ [ CONFIGURATION ] ==================================================*/

#define MAX_FILES 6
#define NUM_DISPLAY_LINES 3

/*================================================= [ DEFINITION ] ==================================================*/

UIMenuService::UIMenuService()
{
  Serial.println("[DEBUG] UIMenuService constructor");
}

/*<===================================================>*/

UIMenuService &UIMenuService::getInstance()
{
  static UIMenuService instance;
  return instance;
}

/*<===================================================>*/

void UIMenuService::setup()
{
  // Set phần cứng liên quan
  LcdHAL::getInstance().setup();
  RotaryEncoderHAL::getInstance().setup();
  BuzzerHAL::getInstance().setup();
  ButtonHAL::getInstance().setup();

  Serial.println("Setting Up...");
  BuzzerHAL::getInstance().startingSoundBuzzer();
  LcdHAL::getInstance().welcomeScreen();
  delay(2000);

  LcdHAL::getInstance().clear();
  LcdHAL::getInstance().modeScreen(modeFlag);

  // Logging
  Serial.println("[SET UP]: UI Menu Done!");
}

/*<===================================================>*/

// bool UIMenuService::cancelSignal()
// {
//   if (Button.isButtonPressed() == true)
//   {
//     Buzzer.beepOnce();
//     currentState = MAIN_MENU;
//     return true;
//   }
//   return false;
// }

/*<===================================================>*/

void UIMenuService::runSDMenu()
{
  uint8_t ar_idx = 0;
  int startIndex = 0;
  int fileCount = MAX_FILES + 1;
  int state = State::SD_MENU;

  LcdHAL::getInstance().clear();
  Serial.printf("currentState: %d\n", state);
  LcdHAL::getInstance().sdModeScreen(fileList, 0, ar_idx, startIndex, fileCount, state);

  while (1)
  {
    if (state == State::AUTO_MODE)
    {
      currentState = AUTO_MODE;
      break;
    }

    RotaryEncoderHAL::getInstance().readEncoder();

    if (RotaryEncoderHAL::getInstance().scrollUp())
    {
      BuzzerHAL::getInstance().beepOnce();
      LcdHAL::getInstance().clear();
      LcdHAL::getInstance().sdModeScreen(fileList, -1, ar_idx, startIndex, fileCount, state);
    }
    else if (RotaryEncoderHAL::getInstance().scrollDown())
    {
      BuzzerHAL::getInstance().beepOnce();
      LcdHAL::getInstance().clear();
      LcdHAL::getInstance().sdModeScreen(fileList, 1, ar_idx, startIndex, fileCount, state);
    }
    else if (RotaryEncoderHAL::getInstance().isRotaryPressed())
    {
      BuzzerHAL::getInstance().beepOnce();
      LcdHAL::getInstance().sdModeScreen(fileList, 2, ar_idx, startIndex, fileCount, state);
    }
  }
}

/*<===================================================>*/

void UIMenuService::runMainMenu()
{
  LcdHAL::getInstance().clear();
  LcdHAL::getInstance().modeScreen(modeFlag);

  while (1)
  {
    RotaryEncoderHAL::getInstance().readEncoder();

    if (RotaryEncoderHAL::getInstance().scrollUp() || RotaryEncoderHAL::getInstance().scrollDown())
    {
      BuzzerHAL::getInstance().beepOnce();
      modeFlag ^= 1; // Toggle nhanh
      LcdHAL::getInstance().clear();
      LcdHAL::getInstance().modeScreen(modeFlag);
    }

    if (RotaryEncoderHAL::getInstance().isRotaryPressed())
    {
      BuzzerHAL::getInstance().beepOnce();
      currentState = (modeFlag == AUTOMODE) ? AUTO_MODE : MANUAL_MODE;
      break;
    }
  }
}

/*<===================================================>*/

void UIMenuService::runAutoMode()
{
  LcdHAL::getInstance().clear();
  LcdHAL::getInstance().automodeScreen(autoModeFlag);

  while (1)
  {
    RotaryEncoderHAL::getInstance().readEncoder();

    if (RotaryEncoderHAL::getInstance().scrollUp() || RotaryEncoderHAL::getInstance().scrollDown())
    {
      BuzzerHAL::getInstance().beepOnce();
      bool isUp = RotaryEncoderHAL::getInstance().scrollUp();
      autoModeFlag = (isUp) ? (autoModeFlag + 2) % 3 : (autoModeFlag + 1) % 3;
      LcdHAL::getInstance().clear();
      LcdHAL::getInstance().automodeScreen(autoModeFlag);
    }

    if (RotaryEncoderHAL::getInstance().isRotaryPressed())
    {
      BuzzerHAL::getInstance().beepOnce();
      switch (autoModeFlag)
      {
      case UGS:
        currentState = UGS_MENU;
        break;
      case SD:
        currentState = SD_MENU;
        break;
      default:
        currentState = MAIN_MENU;
      }
      break;
    }
  }
}

/*<===================================================>*/

void UIMenuService::runManualMode()
{
  LcdHAL::getInstance().clear();
  while (1)
  {
    RotaryEncoderHAL::getInstance().readEncoder();
    LcdHAL::getInstance().lcdDisplay("Manual Mode", LcdHAL::getInstance().getMiddleXCursor("Manual Mode"), 1);
    // TODO: thêm logic nếu có cancel/exit
  }
}

/*<===================================================>*/

void UIMenuService::runUGSMenu()
{
  LcdHAL::getInstance().clear();
  LcdHAL::getInstance().serialModeScreen("retobots logo", 10, workingFlag);

  while (1)
  {
    RotaryEncoderHAL::getInstance().readEncoder();

    if (RotaryEncoderHAL::getInstance().scrollUp() || RotaryEncoderHAL::getInstance().scrollDown())
    {
      BuzzerHAL::getInstance().beepOnce();
      bool isUp = RotaryEncoderHAL::getInstance().scrollUp();
      workingFlag = (isUp) ? (workingFlag + 1) % 3 : (workingFlag + 2) % 3;
      LcdHAL::getInstance().clear();
      LcdHAL::getInstance().serialModeScreen("retobots logo", 10, workingFlag);
    }

    if (RotaryEncoderHAL::getInstance().isRotaryPressed())
    {
      BuzzerHAL::getInstance().beepOnce();
      if (workingFlag == WORKING_STATE)
      {
        if (workingStateFlag == PAUSE)
        {
          Serial.println("Pause");
          workingStateFlag = CONTINUE;
        }
        else
        {
          Serial.println("Continue");
          workingStateFlag = PAUSE;
        }
      }
      else if (workingFlag == CANCEL)
      {
        Serial.println("Cancel");
      }
      else
      {
        currentState = AUTO_MODE;
        break;
      }
    }
  }
}

/*<===================================================>*/

void UIMenuService::run()
{
  switch (currentState)
  {
  case MAIN_MENU:
    runMainMenu();
    break;
  case AUTO_MODE:
    runAutoMode();
    break;
  case MANUAL_MODE:
    runManualMode();
    break;
  case UGS_MENU:
    runUGSMenu();
    break;
  case SD_MENU:
    runSDMenu();
    break;
  }
}

/*<===================================================>*/

void UIMenuService::opening()
{
  BuzzerHAL::getInstance().startingSoundBuzzer();
  LcdHAL::getInstance().welcomeScreen();
  delay(2000);

  LcdHAL::getInstance().clear();
  LcdHAL::getInstance().modeScreen(modeFlag);

  Serial.println("Opening Done!");
}