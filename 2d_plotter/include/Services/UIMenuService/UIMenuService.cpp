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

void UIMenuService::welcomeScreen()
{
  String wel1 = "2DPLOTER";
  String wel2 = "RETOBOTS";
  String wel3 = "WELCOME ANH KHAI";

  // Màn hình 1
  IoHwAb_LCD::getInstance().lcdDisplay(wel1, IoHwAb_LCD::getInstance().getMiddleXCursor(wel1), 1);
  IoHwAb_LCD::getInstance().lcdDisplay(wel2, IoHwAb_LCD::getInstance().getMiddleXCursor(wel2), 2);

  // Màn hình 2
  delay(2000);
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().lcdDisplay(wel3, IoHwAb_LCD::getInstance().getMiddleXCursor(wel3), 1);
}

/*<===================================================>*/

void UIMenuService::modeScreen(uint8_t update)
{
  String options[2] = {"AUTO MODE", "MANUAL MODE"};
  for (int i = 0; i < 2; i++)
  {
    String suffix = (i == update) ? " <" : "";
    IoHwAb_LCD::getInstance().lcdDisplay(options[i] + suffix, 0, i + 1);
  }
}

/*<===================================================>*/

void UIMenuService::automodeScreen(uint8_t update)
{
  String options[3] = {
      "UGS SERIAL",
      "SD CARD",
      "Back"};

  for (int i = 0; i < 3; i++)
  {
    String prefix = (i == update) ? "> " : "  ";
    IoHwAb_LCD::getInstance().lcdDisplay(prefix + options[i], 0, i + 1);
  }
}

/*<===================================================>*/

void UIMenuService::sdModeScreen(vector<String> files, int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state, String &filename)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHOOSE FILE:");

  files[0] = "Back";
  String arrow = ">> ";
  int selectedFile = startIndex + ar_idx;

  /*=======DEBUG======*/
  // Serial.println("--------------------------------------");
  // Serial.printf("Selected file: %d\nStart Index: %d\nArrow Index: %d\nState: %d\n",
  //               selectedFile, startIndex, ar_idx, state);
  // Serial.println("-------**************--------");

  /*--- Xử lý dịch chỉ số ---*/
  if (signal == 1)
  { // Scroll xuống
    if (ar_idx < 2)
    {
      ar_idx++;
    }
    else
    {
      if (selectedFile == fileCount - 1)
      {
        startIndex = (selectedFile + 1) % fileCount;
        ar_idx = 0;
      }
      else
      {
        startIndex = (startIndex + 1) % fileCount;
      }
    }
  }
  else if (signal == -1)
  { // Scroll lên
    if (ar_idx > 0)
    {
      ar_idx--;
    }
    else
    {
      if (startIndex == 0)
      {
        startIndex = (fileCount + startIndex - 3) % fileCount;
        ar_idx = 2;
      }
      else
      {
        startIndex = (startIndex + fileCount - 1) % fileCount;
      }
    }
  }
  else if (signal == 2)
  { // Nhấn chọn
    if (selectedFile == 0)
    {
      Serial.println("Back");
      state = 1;
      return;
    }
    else
    {
      IoHwAb_LCD::getInstance().clear();
      IoHwAb_LCD::getInstance().lcdDisplay(files[selectedFile] + " chose", IoHwAb_LCD::getInstance().getMiddleXCursor(files[selectedFile] + " chose"), 1);
      delay(1500);
      IoHwAb_LCD::getInstance().clear();
      filename = files[selectedFile];
      state = 5; // Set state to indicate file selection
      // IoHwAb_LCD::getInstance().lcdDisplay("Read file", IoHwAb_LCD::getInstance().getMiddleXCursor("Read file"), 1);
      return;
    }
  }

  // --- Hiển thị 3 dòng với mũi tên ---
  for (int i = 0; i < 3; i++)
  {
    int index = (startIndex + i) % fileCount;
    String prefix = (i == ar_idx) ? arrow : "   ";
    IoHwAb_LCD::getInstance().lcdDisplay(prefix + files[index], 0, i + 1);
  }

  /*========DEBUG=========*/
  selectedFile = startIndex + ar_idx;
  //   Serial.println("--------------------------------------");
  //   Serial.printf("Selected file: %d\nStart Index: %d\nArrow Index: %d\nState: %d\n",
  //                 selectedFile, startIndex, ar_idx, state);
  //   Serial.println("-------**************--------");
}

/*<===================================================>*/

void UIMenuService::serialModeScreen(uint8_t workingStateMode)
{
  String title = "RGSA PLATFORM";
  // String percentage = "  % : " + String(percenum) + "%";
  // String filename = "File: " + file;

  String options[3] = {"PAUSE", "CANCEL", "Back"};
  int positions[3] = {0, 7, 15};

  IoHwAb_LCD::getInstance().lcdDisplay(title, IoHwAb_LCD::getInstance().getMiddleXCursor(title), 0);

  for (int i = 0; i < 3; i++)
  {
    String prefix = (i == workingStateMode) ? ">" : " ";
    IoHwAb_LCD::getInstance().lcdDisplay(prefix + options[i], positions[i], 3);
  }
}

void UIMenuService::UGSModeScreen()
{
  String title = "RGSA PLATFORM";

  IoHwAb_LCD::getInstance().lcdDisplay(title, IoHwAb_LCD::getInstance().getMiddleXCursor(title), 1);

  IoHwAb_LCD::getInstance().lcdDisplay("> Back", 14, 3);
}

/*<===================================================>*/

void UIMenuService::loadingScreen()
{
  String loading = "--LOADING.--";
  IoHwAb_LCD::getInstance().lcdDisplay(loading, IoHwAb_LCD::getInstance().getMiddleXCursor(loading), 1);
}

/*<===================================================>*/

void UIMenuService::statusScreen(String filename, int percenum, String time, uint8_t workingStateMode, uint8_t workingStateFlag)
{
  String title = "File: " + filename;
  String percentage = "  % : " + String(percenum) + "%";
  String timeDisplay = "Time: " + time;

  // First option depends on pause/continue flag (0: PAUSE, 1: CONTINUE)
  String option0 = (workingStateFlag == 0) ? "PAUSE" : "RESUME";
  String options[3] = {option0, "CANCEL", "Back"};
  int positions[3] = {0, 7, 15};

  IoHwAb_LCD::getInstance().lcdDisplay(title, 0, 0);
  IoHwAb_LCD::getInstance().lcdDisplay(percentage, 0, 1);
  IoHwAb_LCD::getInstance().lcdDisplay(timeDisplay, 0, 2);

  for (int i = 0; i < 3; i++)
  {
    String prefix = (i == workingStateMode) ? ">" : " ";
    IoHwAb_LCD::getInstance().lcdDisplay(prefix + options[i], positions[i], 3);
  }
}

void UIMenuService::PS4ModeScreen(int choice)
{
  String title = "MANUAL MODE";
  String macAdress = IoHwAb_PS4::getInstance().getMacAdress();

  IoHwAb_LCD::getInstance().lcdDisplay(title, IoHwAb_LCD::getInstance().getMiddleXCursor(title), 0);

  // Options Sections

  if (choice == 0)
  {
    IoHwAb_LCD::getInstance().lcdDisplay("MAC Address", IoHwAb_LCD::getInstance().getMiddleXCursor("MAC Address"), 1);
    IoHwAb_LCD::getInstance().lcdDisplay(macAdress, IoHwAb_LCD::getInstance().getMiddleXCursor(macAdress), 2);
  }

  else if (choice == 1)
  {
    IoHwAb_LCD::getInstance().lcdDisplay("Connected", IoHwAb_LCD::getInstance().getMiddleXCursor("Connected"), 1);
  }

  else if (choice == 2)
  {
    IoHwAb_LCD::getInstance().lcdDisplay("Not Connected", IoHwAb_LCD::getInstance().getMiddleXCursor("Not Connected"), 1);
  }

  IoHwAb_LCD::getInstance().lcdDisplay("> Back", 14, 3);
}