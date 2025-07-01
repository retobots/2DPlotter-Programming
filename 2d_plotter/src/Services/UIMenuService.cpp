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

  Serial.println("Setting Up...");
  Buzzer.startingSoundBuzzer();
  LCD.welcomeScreen();
  delay(2000);

  LCD.clear();
  LCD.modeScreen(modeFlag);
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

void UIMenuService::run()
{

  switch (currentState)
  {
  case MAIN_MENU:
    LCD.clear();
    LCD.modeScreen(modeFlag);
    while (1)
    {
      RE.readEncoder();
      if (RE.scrollUp() || RE.scrollDown())
      {
        Buzzer.beepOnce();
        modeFlag = 1 - modeFlag;
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
    break;

  case AUTO_MODE:
    // Xử lý giao diện chế độ tự động ở đây
    // Ví dụ: Chọn UGS hay SD
    LCD.clear();
    LCD.automodeScreen(autoModeFlag);
    while (1)
    {
      RE.readEncoder();
      // if (cancelSignal() == true)
      //   break;
      if (RE.scrollDown())
      {
        Buzzer.beepOnce();
        autoModeFlag = (autoModeFlag + 1) % 3;
        LCD.clear();
        LCD.automodeScreen(autoModeFlag);
      }
      else if (RE.scrollUp())
      {
        Buzzer.beepOnce();
        autoModeFlag = (autoModeFlag + 2) % 3;
        LCD.clear();
        LCD.automodeScreen(autoModeFlag);
      }
      if (RE.isRotaryPressed())
      {
        Buzzer.beepOnce();
        if (autoModeFlag == UGS)
        {
          currentState = UGS_MENU;
        }
        else if (autoModeFlag == SD)
        {
          currentState = SD_MENU;
        }
        else
        {
          currentState = MAIN_MENU;
        }
        break;
      }
    }
    break;

  case MANUAL_MODE:
    LCD.clear();
    while (1)
    {
      RE.readEncoder();
      // if (cancelSignal() == true)
      //   break;

      LCD.lcdDisplay("Manual Mode", LCD.getMiddleXCursor("Manual Mode"), 1);
    }
    break;

  case UGS_MENU:
    LCD.clear();
    LCD.serialModeScreen("retobots logo", 10, workingFlag);
    while (1)
    {
      RE.readEncoder();
      // if (cancelSignal() == true)
      //   break;

      if (RE.scrollUp())
      {
        Buzzer.beepOnce();
        workingFlag = (workingFlag + 1) % 3;
        LCD.clear();
        LCD.serialModeScreen("retobots logo", 10, workingFlag);
      }
      else if (RE.scrollDown())
      {
        Buzzer.beepOnce();
        workingFlag = (workingFlag + 2) % 3;
        LCD.clear();
        LCD.serialModeScreen("retobots logo", 10, workingFlag);
      }

      if (RE.isRotaryPressed())
      {
        Buzzer.beepOnce();
        if ((workingFlag == WORKING_STATE) && (workingStateFlag == PAUSE))
        {
          Serial.println("Tạm dừng");
          workingStateFlag = CONTINUE;
        }
        else if ((workingFlag == WORKING_STATE) && (workingStateFlag == CONTINUE))
        {
          Serial.println("Tiếp tục");
          workingStateFlag = PAUSE;
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
    break;

  case SD_MENU:
    // loadFileListFromSD();
    uint8_t ar_idx = 0;
    int startIndex = 0;
    int fileCount = MAX_FILES + 1;
    LCD.clear();
    int state = State::SD_MENU;
    Serial.print("currentState: ");
    Serial.println(state);
    LCD.sdModeScreen(fileList, 0, ar_idx, startIndex, fileCount, state);
    while (1)
    {
      if (state == State::AUTO_MODE)
      {
        currentState = AUTO_MODE;
        break;
      }
      else
      {
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
    break;
  }
}

/*<===================================================>*/