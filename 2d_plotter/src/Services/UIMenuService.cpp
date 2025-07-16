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

String fileList[MAX_FILES + 1] = {
    "",
    "log1.txt",
    "config.csv",
    "data_A.bin",
    "log2.txt",
    "hello.csv",
    "report.md"};

/*================================================= [ DEFINITION ] ==================================================*/

UIMenuService::UIMenuService()
    : LCD(LcdHAL::getInstance()),
      RE(RotaryEncoderHAL::getInstance()),
      Buzzer(BuzzerHAL::getInstance()),
      Button(ButtonHAL::getInstance())
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
  LCD.setup();
  RE.setup();
  Buzzer.setup();
  Button.setup();

  Serial.println("Setting Up...");
  Buzzer.startingSoundBuzzer();
  LCD.welcomeScreen();
  delay(2000);

  LCD.clear();
  LCD.modeScreen(modeFlag);

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

  LCD.clear();
  Serial.printf("currentState: %d\n", state);
  LCD.sdModeScreen(fileList, 0, ar_idx, startIndex, fileCount, state);

  while (1)
  {
    if (state == State::AUTO_MODE)
    {
      currentState = AUTO_MODE;
      break;
    }

    RE.readEncoder();

    if (RE.scrollUp())
    {
      Buzzer.beepOnce();
      LCD.clear();
      LCD.sdModeScreen(fileList, -1, ar_idx, startIndex, fileCount, state);
    }
    else if (RE.scrollDown())
    {
      Buzzer.beepOnce();
      LCD.clear();
      LCD.sdModeScreen(fileList, 1, ar_idx, startIndex, fileCount, state);
    }
    else if (RE.isRotaryPressed())
    {
      Buzzer.beepOnce();
      LCD.sdModeScreen(fileList, 2, ar_idx, startIndex, fileCount, state);
    }
  }
}

/*<===================================================>*/

void UIMenuService::runMainMenu()
{
  LCD.clear();
  LCD.modeScreen(modeFlag);

  while (1)
  {
    RE.readEncoder();

    if (RE.scrollUp() || RE.scrollDown())
    {
      Buzzer.beepOnce();
      modeFlag ^= 1; // Toggle nhanh
      LCD.clear();
      LCD.modeScreen(modeFlag);
    }

    if (RE.isRotaryPressed())
    {
      Buzzer.beepOnce();
      currentState = (modeFlag == AUTOMODE) ? AUTO_MODE : MANUAL_MODE;
      break;
    }
  }
}

/*<===================================================>*/

void UIMenuService::runAutoMode()
{
  LCD.clear();
  LCD.automodeScreen(autoModeFlag);

  while (1)
  {
    RE.readEncoder();

    if (RE.scrollUp() || RE.scrollDown())
    {
      Buzzer.beepOnce();
      bool isUp = RE.scrollUp();
      autoModeFlag = (isUp) ? (autoModeFlag + 2) % 3 : (autoModeFlag + 1) % 3;
      LCD.clear();
      LCD.automodeScreen(autoModeFlag);
    }

    if (RE.isRotaryPressed())
    {
      Buzzer.beepOnce();
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
  LCD.clear();
  while (1)
  {
    RE.readEncoder();
    LCD.lcdDisplay("Manual Mode", LCD.getMiddleXCursor("Manual Mode"), 1);
    // TODO: thêm logic nếu có cancel/exit
  }
}

/*<===================================================>*/

void UIMenuService::runUGSMenu()
{
  LCD.clear();
  LCD.serialModeScreen("retobots logo", 10, workingFlag);

  while (1)
  {
    RE.readEncoder();

    if (RE.scrollUp() || RE.scrollDown())
    {
      Buzzer.beepOnce();
      bool isUp = RE.scrollUp();
      workingFlag = (isUp) ? (workingFlag + 1) % 3 : (workingFlag + 2) % 3;
      LCD.clear();
      LCD.serialModeScreen("retobots logo", 10, workingFlag);
    }

    if (RE.isRotaryPressed())
    {
      Buzzer.beepOnce();
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
  Buzzer.startingSoundBuzzer();
  LCD.welcomeScreen();
  delay(2000);

  LCD.clear();
  LCD.modeScreen(modeFlag);

  Serial.println("Opening Done!");
}