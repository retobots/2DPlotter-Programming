/*******************************************************************************
 * @file    IoHwAb_LCD.cpp
 * @brief   Setup thông số cho LCD. Định nghĩa các hàm hiển thị các màn hình menu
 * @version 1.0
 * @date    2025-06-19
 * @author  Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "IoHwAb_LCD.h"

/*================================================= [ DEFINITION ] ==================================================*/

LiquidCrystal_I2C lcd(0x27, 20, 4); // 0x27 là địa chỉ I2C của màn hình (có thể là 0x3F)

/*<===================================================>*/

IoHwAb_LCD::IoHwAb_LCD() {}

/*<===================================================>*/

IoHwAb_LCD &IoHwAb_LCD::getInstance()
{
  static IoHwAb_LCD instance;
  return instance;
}

/*<===================================================>*/

void IoHwAb_LCD::setup()
{
  // Khởi tạo LCD (I2C hoặc SPI)
  lcd.init();      // Khởi tạo LCD
  lcd.backlight(); // Bật đèn nền

  delay(500);

  // Logging
  Serial.println("[SET UP]: LCD Done!");
}

/*<===================================================>*/

void IoHwAb_LCD::clear()
{
  // Xoá màn hình
  lcd.clear();
}

/*<===================================================>*/

int IoHwAb_LCD::getMiddleXCursor(const String &text)
{
  int strLen = text.length(); // Độ dài chuỗi
  int lcdWidth = 20;          // LCD 20 cột
  int startCol = (lcdWidth - strLen) / 2;
  if (startCol < 0)
    startCol = 0; // Tránh âm nếu text quá dài
  return startCol;
}

/*<===================================================>*/

void IoHwAb_LCD::lcdDisplay(const String &text, uint8_t x, uint8_t y)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}

/*<===================================================>*/

void IoHwAb_LCD::welcomeScreen()
{
  String wel1 = "2DPLOTER";
  String wel2 = "RETOBOTS";
  String wel3 = "WELCOME ANH KHAI";

  // Màn hình 1
  lcdDisplay(wel1, getMiddleXCursor(wel1), 1);
  lcdDisplay(wel2, getMiddleXCursor(wel2), 2);

  // Màn hình 2
  delay(2000);
  IoHwAb_LCD::clear();
  lcdDisplay(wel3, getMiddleXCursor(wel3), 1);
}

/*<===================================================>*/

void IoHwAb_LCD::modeScreen(uint8_t update)
{
  String options[2] = {"AUTO MODE", "MANUAL MODE"};
  for (int i = 0; i < 2; i++)
  {
    String suffix = (i == update) ? " <" : "";
    lcdDisplay(options[i] + suffix, 0, i + 1);
  }
}

/*<===================================================>*/

void IoHwAb_LCD::automodeScreen(uint8_t update)
{
  String options[3] = {
      "UGS SERIAL",
      "SD CARD",
      "Back"};

  for (int i = 0; i < 3; i++)
  {
    String prefix = (i == update) ? "> " : "  ";
    lcdDisplay(prefix + options[i], 0, i + 1);
  }
}

/*<===================================================>*/

void IoHwAb_LCD::sdModeScreen(vector<String> files, int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHOOSE FILE:");

  files[0] = "Back";
  String arrow = ">> ";
  int selectedFile = startIndex + ar_idx;

  /*=======DEBUG======*/
  Serial.println("--------------------------------------");
  Serial.printf("Selected file: %d\nStart Index: %d\nArrow Index: %d\nState: %d\n",
                selectedFile, startIndex, ar_idx, state);
  Serial.println("-------**************--------");

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
      lcd.clear();
      lcdDisplay(files[selectedFile] + " chose", getMiddleXCursor(files[selectedFile] + " chose"), 1);
      delay(1500);
      lcd.clear();
      lcdDisplay("Read file", getMiddleXCursor("Read file"), 1);
      return;
    }
  }

  // --- Hiển thị 3 dòng với mũi tên ---
  for (int i = 0; i < 3; i++)
  {
    int index = (startIndex + i) % fileCount;
    String prefix = (i == ar_idx) ? arrow : "   ";
    lcdDisplay(prefix + files[index], 0, i + 1);
  }

  /*========DEBUG=========*/
  selectedFile = startIndex + ar_idx;
  Serial.println("--------------------------------------");
  Serial.printf("Selected file: %d\nStart Index: %d\nArrow Index: %d\nState: %d\n",
                selectedFile, startIndex, ar_idx, state);
  Serial.println("-------**************--------");
}

/*<===================================================>*/

void IoHwAb_LCD::serialModeScreen(uint8_t workingStateMode)
{
  String title = "RCSA PLATFORM";
  // String percentage = "  % : " + String(percenum) + "%";
  // String filename = "File: " + file;

  String options[3] = {"PAUSE", "CANCEL", "Back"};
  int positions[3] = {0, 7, 15};

  lcdDisplay(title, getMiddleXCursor(title), 0);
  // lcdDisplay(filename, 0, 1);
  // lcdDisplay(percentage, 0, 2);

  for (int i = 0; i < 3; i++)
  {
    String prefix = (i == workingStateMode) ? ">" : " ";
    lcdDisplay(prefix + options[i], positions[i], 3);
  }
}

/*<===================================================>*/

void IoHwAb_LCD::loadingScreen()
{
  String loading = "--LOADING.--";
  lcdDisplay(loading, getMiddleXCursor(loading), 1);
}

/*<===================================================>*/