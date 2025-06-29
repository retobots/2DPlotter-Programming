/*******************************************************************************
 * @file    LcdHAL.cpp
 * @brief   Setup thông số cho LCD. Định nghĩa các hàm hiển thị các màn hình menu
 * @version 1.0
 * @date    2025-06-19
 * @author  Duc Nghia
 ******************************************************************************/

/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "LcdHAL.h"

/*================================================= [ DEFINITION ] ==================================================*/

LiquidCrystal_I2C lcd(0x27, 20, 4); // 0x27 là địa chỉ I2C của màn hình (có thể là 0x3F)

/*<===================================================>*/

LcdHAL::LcdHAL() { setup(); }

/*<===================================================>*/

LcdHAL &LcdHAL::getInstance()
{
  Serial.println("INSTANCE LCD CREATED");
  static LcdHAL instance;
  return instance;
}

/*<===================================================>*/

void LcdHAL::setup()
{
  // Khởi tạo LCD (I2C hoặc SPI)
  lcd.init();      // Khởi tạo LCD
  lcd.backlight(); // Bật đèn nền

  delay(500);
}

/*<===================================================>*/

void LcdHAL::clear()
{
  // Xoá màn hình
  lcd.clear();
}

/*<===================================================>*/

int LcdHAL::getMiddleXCursor(const String &text)
{
  int strLen = text.length(); // Độ dài chuỗi
  int lcdWidth = 20;          // LCD 20 cột
  int startCol = (lcdWidth - strLen) / 2;
  if (startCol < 0)
    startCol = 0; // Tránh âm nếu text quá dài
  return startCol;
}

/*<===================================================>*/

void LcdHAL::lcdDisplay(const String &text, uint8_t x, uint8_t y)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}

/*<===================================================>*/

void LcdHAL::welcomeScreen()
{
  String wel1 = "2DPLOTER";
  String wel2 = "RETOBOTS";
  String wel3 = "WELCOME ANH KHAI";

  // Màn hình 1
  lcdDisplay(wel1, getMiddleXCursor(wel1), 1);
  lcdDisplay(wel2, getMiddleXCursor(wel2), 2);

  // Màn hình 2
  delay(2000);
  LcdHAL::clear();
  lcdDisplay(wel3, getMiddleXCursor(wel3), 1);
}

/*<===================================================>*/

void LcdHAL::modeScreen(uint8_t update)
{
  String line1 = "AUTO MODE   <";
  String line2 = "MANUAL MODE  ";

  if (update == 0)
  {
    line1 = "AUTO MODE   <";
    line2 = "MANUAL MODE  ";
  }
  else if (update == 1)
  {
    line1 = "AUTO MODE    ";
    line2 = "MANUAL MODE <";
  }

  lcdDisplay(line1, 0, 1);
  lcdDisplay(line2, 0, 2);
}

/*<===================================================>*/

void LcdHAL::automodeScreen(uint8_t update)
{
  String line1 = "UGS SERIAL <";
  String line2 = "SD CARD     ";
  String line3 = "Back";

  if (update == 0)
  {
    line1 = "UGS SERIAL <";
    line2 = "SD CARD     ";
    line3 = "Back";
  }
  else if (update == 1)
  {
    line1 = "UGS SERIAL ";
    line2 = "SD CARD    <";
    line3 = "Back";
  }
  else if (update == 2)
  {
    line1 = "UGS SERIAL ";
    line2 = "SD CARD";
    line3 = "> Back";
  }

  lcdDisplay(line1, 0, 1);
  lcdDisplay(line2, 0, 2);
  lcdDisplay(line3, (19 - line3.length()), 3);
}

/*<===================================================>*/

void LcdHAL::sdModeScreen(String files[], int8_t signal, uint8_t &ar_idx, int &startIndex, int fileCount, int &state)
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

void LcdHAL::serialModeScreen(String file, int percenum, uint8_t workingStateMode)
{
  String title = "UGS PLATFORM";
  String percentage = "  % : " + String(percenum) + "%";
  String filename = "File: " + file;
  String cancel = "> CANCEL";
  String workingState = "PAUSE";
  String back = "Back";

  if (workingStateMode == 1)
  {
    cancel = ">CANCEL";
    workingState = " PAUSE";
    back = " Back";
  }

  else if (workingStateMode == 0)
  {
    cancel = " CANCEL";
    workingState = ">PAUSE";
    back = " Back";
  }
  else if (workingStateMode == 2)
  {
    cancel = " CANCEL";
    workingState = " PAUSE";
    back = ">Back";
  }

  lcdDisplay(title, getMiddleXCursor(title), 0);
  lcdDisplay(filename, 0, 1);
  lcdDisplay(percentage, 0, 2);
  lcdDisplay(workingState, 0, 3);
  lcdDisplay(cancel, 7, 3);
  lcdDisplay(back, 15, 3);
}

/*<===================================================>*/

void LcdHAL::loadingScreen()
{
  String loading = "--LOADING.--";
  lcdDisplay(loading, getMiddleXCursor(loading), 1);
}

/*<===================================================>*/