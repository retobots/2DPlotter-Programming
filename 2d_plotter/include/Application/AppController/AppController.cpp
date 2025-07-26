#include "AppController.h"
#include "AutoModeController.h"
#include "ManualModeController.h"

AppController::AppController()
{
  // Constructor
}

AppController &AppController::getInstance()
{
  static AppController instance;
  return instance;
}

void AppController::setup()
{
  // Khởi tạo các thành phần cần thiết, hiển thị menu
}

/*<===================================================>*/

void AppController::runSDMenu()
{
  uint8_t ar_idx = 0;
  int startIndex = 0;
  int state = State::SD_MENU;

  IoHwAb_LCD::getInstance().clear();
  Serial.printf("currentState: %d\n", state);
  UIMenuService::getInstance().sdModeScreen(fileList, 0, ar_idx, startIndex, fileList.size(), state);

  while (1)
  {
    if (state == State::AUTO_MODE)
    {
      currentState = State::AUTO_MODE;
      break;
    }

    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().sdModeScreen(fileList, -1, ar_idx, startIndex, fileList.size(), state);
    }
    else if (IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().sdModeScreen(fileList, 1, ar_idx, startIndex, fileList.size(), state);
    }
    else if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      UIMenuService::getInstance().sdModeScreen(fileList, 2, ar_idx, startIndex, fileList.size(), state);
    }
  }
}

/*<===================================================>*/

void AppController::runMainMenu()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().modeScreen(modeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      modeFlag ^= 1; // Toggle nhanh
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().modeScreen(modeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      currentState = (modeFlag == AUTOMODE) ? State::AUTO_MODE : State::MANUAL_MODE;
      break;
    }
  }
}

/*<===================================================>*/

void AppController::runAutoMode()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().automodeScreen(autoModeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
      autoModeFlag = (isUp) ? (autoModeFlag + 2) % 3 : (autoModeFlag + 1) % 3;
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().automodeScreen(autoModeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      switch (autoModeFlag)
      {
      case UGS:
        currentState = State::UGS_MENU;
        break;
      case SD:
        currentState = State::SD_MENU;
        break;
      default:
        currentState = State::MAIN_MENU;
      }
      break;
    }
  }
}

/*<===================================================>*/

void AppController::runManualMode()
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

void AppController::runUGSMenu()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().serialModeScreen(workingFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
      workingFlag = (isUp) ? (workingFlag + 1) % 3 : (workingFlag + 2) % 3;
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().serialModeScreen(workingFlag);
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
        currentState = State::AUTO_MODE;
        break;
      }
    }
  }
}

/*<===================================================>*/

void AppController::run()
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

void AppController::opening()
{
  IoHwAb_Buzzer::getInstance().startingSoundBuzzer();
  UIMenuService::getInstance().welcomeScreen();
  delay(2000);

  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().modeScreen(modeFlag);

  Serial.println("Opening Done!");
}
