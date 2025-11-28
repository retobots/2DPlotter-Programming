/*************************************************************************************************************************
 * @file    UIMenuService.cpp
 * @brief   Definitions of functions in the source file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include "UIMenuService.h"

#define MAX_FILES 6
#define NUM_DISPLAY_LINES 3

/*************************************************************************************************************************
 * @brief   Constructor
 ************************************************************************************************************************/
UIMenuService::UIMenuService()
{
  Serial.println("[DEBUG] UIMenuService constructor");
}

/*************************************************************************************************************************
 * @brief   Get the singleton instance of UIMenuService
 * @return  Reference to the UIMenuService instance
 ************************************************************************************************************************/
UIMenuService &UIMenuService::getInstance()
{
  static UIMenuService instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief   Setup the UI Menu Service
 ************************************************************************************************************************/
void UIMenuService::setup()
{
  IoHwAb_LCD::getInstance().setup();
  IoHwAb_Encoder::getInstance().setup();
  IoHwAb_Buzzer::getInstance().setup();
  IoHwAb_Button::getInstance().setup();

  // Logging
  Serial.println("[SET UP]: UI Menu Done!");
}

/*************************************************************************************************************************
 * @brief   Run the UI Menu Service
 ************************************************************************************************************************/
void UIMenuService::welcomeScreen()
{
  String wel1 = "2DPLOTER";
  String wel2 = "RETOBOTS";
  String wel3 = "WELCOME ANH KHAI";

  // Show welcome screen
  IoHwAb_LCD::getInstance().lcdDisplay(wel1, IoHwAb_LCD::getInstance().getMiddleXCursor(wel1), 1);
  IoHwAb_LCD::getInstance().lcdDisplay(wel2, IoHwAb_LCD::getInstance().getMiddleXCursor(wel2), 2);

  // Show second screen
  delay(2000);
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_LCD::getInstance().lcdDisplay(wel3, IoHwAb_LCD::getInstance().getMiddleXCursor(wel3), 1);
}

/*************************************************************************************************************************
 * @brief   Display the mode selection screen
 ************************************************************************************************************************/
void UIMenuService::modeScreen(uint8_t update)
{
  String options[2] = {"AUTO MODE", "MANUAL MODE"};
  for (int i = 0; i < 2; i++)
  {
    String suffix = (i == update) ? " <" : "";
    IoHwAb_LCD::getInstance().lcdDisplay(options[i] + suffix, 0, i + 1);
  }
}

/*************************************************************************************************************************
 * @brief   Display the automode selection screen
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief   Display the SD mode selection screen
 *
 * @param   files         Vector of file names available on the SD card
 * @param   signal        Signal indicating user action (1: down, -1: up, 2: select)
 * @param   ar_idx        Reference to the arrow index for display
 * @param   startIndex    Reference to the start index for file listing
 * @param   fileCount     Total number of files available
 * @param   state         Reference to the current state variable
 ************************************************************************************************************************/
void UIMenuService::sdModeScreen(vector<String> files, int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state, String &filename)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHOOSE FILE:");

  files[0] = "Back";
  String arrow = ">> ";
  int selectedFile = startIndex + ar_idx;

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
  {
    // Scroll up
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
  {
    // Select file
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
      return;
    }
  }

  for (int i = 0; i < 3; i++)
  {
    int index = (startIndex + i) % fileCount;
    String prefix = (i == ar_idx) ? arrow : "   ";
    IoHwAb_LCD::getInstance().lcdDisplay(prefix + files[index], 0, i + 1);
  }

  selectedFile = startIndex + ar_idx;
}

/*************************************************************************************************************************
 * @brief   Display the serial mode selection screen
 *
 * @param   workingStateMode   Current working state mode index
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief   Display the UGS mode selection screen
 ************************************************************************************************************************/
void UIMenuService::UGSModeScreen()
{
  String title = "RGSA PLATFORM";

  IoHwAb_LCD::getInstance().lcdDisplay(title, IoHwAb_LCD::getInstance().getMiddleXCursor(title), 1);

  IoHwAb_LCD::getInstance().lcdDisplay("> Back", 14, 3);
}

/*************************************************************************************************************************
 * @brief   Display the loading screen
 ************************************************************************************************************************/
void UIMenuService::loadingScreen()
{
  String loading = "--LOADING.--";
  IoHwAb_LCD::getInstance().lcdDisplay(loading, IoHwAb_LCD::getInstance().getMiddleXCursor(loading), 1);
}

/*************************************************************************************************************************
 * @brief   Display the status screen
 *
 * @param   filename            Name of the file being processed
 * @param   percenum            Percentage of completion
 * @param   time                Elapsed time
 * @param   workingStateMode    Current working state mode index
 * @param   workingStateFlag    Pause/continue flag (0: PAUSE, 1: CONTINUE)
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief   Display the PS4 mode selection screen
 *
 * @param   choice    Current choice index
 ************************************************************************************************************************/
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