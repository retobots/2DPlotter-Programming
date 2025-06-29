#include "HAL/SDCardHAL.h"

SDCardHAL::SDCardHAL()
{
}

SDCardHAL &SDCardHAL::getInstance()
{
  static SDCardHAL instance;
  return instance;
}

void SDCardHAL::setup()
{
  // Mount thẻ SD
}

std::vector<std::string> SDCardHAL::listFiles()
{
  // Trả về danh sách file .gcode
  return {};
}

bool SDCardHAL::openFile(const std::string &filename)
{
  // Mở file
  return true;
}

std::string SDCardHAL::readLine()
{
  // Đọc 1 dòng G-code
  return "";
}

void SDCardHAL::closeFile()
{
  // Đóng file
}

// void SDCardHAL::loadFileListFromSD()
// {
//   fileCount = 0;
//   File root = SD.open("/");

//   while (true)
//   {
//     File entry = root.openNextFile();
//     if (!entry)
//       break;
//     if (!entry.isDirectory() && fileCount < MAX_FILES)
//     {
//       fileList[fileCount++] = String(entry.name());
//     }
//     entry.close();
//   }
// }

// void readSelectedFile(String filename)
// {
//   File f = SD.open(filename.c_str());
//   if (!f)
//   {
//     Serial.println("Không mở được file!");
//     return;
//   }

//   Serial.println("Đang đọc nội dung:");
//   while (f.available())
//   {
//     Serial.write(f.read());
//   }

//   f.close();
// }