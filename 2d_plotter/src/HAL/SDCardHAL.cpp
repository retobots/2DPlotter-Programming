#include "HAL/SDCardHAL.h"
using namespace std;

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
  SD.begin(PIN_SD_CS);
}

string SDCardHAL::readLine()
{
  // Đọc 1 dòng G-code
  return "";
}

void SDCardHAL::loadFileListFromSD()
{
  fileNames.clear();
  // Add "Back" as the first element
  fileNames.push_back("Back");

  File root = SD.open("/");
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
      break;
    if (!entry.isDirectory())
    {
      std::string fname = entry.name();
      fileNames.push_back(fname);
    }
    entry.close();
  }
}

void SDCardHAL::readSelectedFile(const std::string &filename)
{
  fileContents.clear();
  File f = SD.open(filename.c_str());
  if (!f)
  {
    Serial.println("Cannot open file!");
    return;
  }

  std::string line;
  while (f.available())
  {
    char c = f.read();
    if (c == '\n' || c == '\r')
    {
      if (!line.empty())
      {
        fileContents.push_back(line);
        line.clear();
      }
    }
    else
    {
      line += c;
    }
  }
  if (!line.empty())
  {
    fileContents.push_back(line);
  }
  f.close();
}