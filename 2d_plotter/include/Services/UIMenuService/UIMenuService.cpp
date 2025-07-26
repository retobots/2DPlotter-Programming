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
  IoHwAb_LCD::getInstance().setup();
  IoHwAb_Encoder::getInstance().setup();
  IoHwAb_Buzzer::getInstance().setup();
  IoHwAb_Button::getInstance().setup();

  Serial.println("Setting Up...");
  IoHwAb_Buzzer::getInstance().startingSoundBuzzer();
  IoHwAb_LCD::getInstance().welcomeScreen();
  delay(2000);

  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().modeScreen(modeFlag);

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
  int state = State::SD_MENU;

  IoHwAb_LCD::getInstance().clear();
  Serial.printf("currentState: %d\n", state);
  IoHwAb_LCD::getInstance().sdModeScreen(fileList, 0, ar_idx, startIndex, fileList.size(), state);

  while (1)
  {
    if (state == State::AUTO_MODE)
    {
      currentState = AUTO_MODE;
      break;
    }

    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().sdModeScreen(fileList, -1, ar_idx, startIndex, fileList.size(), state);
    }
    else if (IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().sdModeScreen(fileList, 1, ar_idx, startIndex, fileList.size(), state);
    }
    else if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().sdModeScreen(fileList, 2, ar_idx, startIndex, fileList.size(), state);
    }
  }
}

/*<===================================================>*/

void UIMenuService::runMainMenu()
{
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().modeScreen(modeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      modeFlag ^= 1; // Toggle nhanh
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().modeScreen(modeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      currentState = (modeFlag == AUTOMODE) ? AUTO_MODE : MANUAL_MODE;
      break;
    }
  }
}

/*<===================================================>*/

void UIMenuService::runAutoMode()
{
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().automodeScreen(autoModeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
      autoModeFlag = (isUp) ? (autoModeFlag + 2) % 3 : (autoModeFlag + 1) % 3;
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().automodeScreen(autoModeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
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
  IoHwAb_LCD::getInstance().clear();
  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();
    IoHwAb_LCD::getInstance().lcdDisplay("Manual Mode", IoHwAb_LCD::getInstance().getMiddleXCursor("Manual Mode"), 1);
    // TODO: thêm logic nếu có cancel/exit
  }
}

/*<===================================================>*/

void UIMenuService::runUGSMenu()
{
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().serialModeScreen(workingFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
      workingFlag = (isUp) ? (workingFlag + 1) % 3 : (workingFlag + 2) % 3;
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().serialModeScreen(workingFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
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
  IoHwAb_Buzzer::getInstance().startingSoundBuzzer();
  IoHwAb_LCD::getInstance().welcomeScreen();
  delay(2000);

  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().modeScreen(modeFlag);

  Serial.println("Opening Done!");
}