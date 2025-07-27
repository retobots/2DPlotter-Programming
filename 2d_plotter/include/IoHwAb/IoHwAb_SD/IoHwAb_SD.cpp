#include "IoHwAb_SD.h"
using namespace std;

IoHwAb_SD::IoHwAb_SD()
{
  // Constructor logic if needed
}

IoHwAb_SD &IoHwAb_SD::getInstance()
{
  static IoHwAb_SD instance;
  return instance;
}

void IoHwAb_SD::setup()
{
  SD.begin(PIN_SD_CS);
}

void IoHwAb_SD::loadFileListFromSD()
{
  fileList.clear();
  // Add "Back" as the first element
  fileList.push_back("Back");

  File root = SD.open("/");
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
      break;
    if (!entry.isDirectory())
    {
      String fname = entry.name();
      fileList.push_back(fname);
    }
    entry.close();
  }
}

void IoHwAb_SD::readSelectedFile(String &filename)
{
  fileContents.clear();
  File f = SD.open(filename.c_str());
  if (!f)
  {
    Serial.println("Cannot open file!");
    return;
  }

  String line;
  while (f.available())
  {
    char c = f.read();
    if (c == '\n' || c == '\r')
    {
      if (!line.isEmpty())
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
  if (!line.isEmpty())
  {
    fileContents.push_back(line);
  }
  f.close();
}

File &IoHwAb_SD::getFile(const String &filename)
{
  static File file;
  file = SD.open(filename.c_str());
  if (!file)
  {
    Serial.println("Cannot open file!");
  }
  return file;
}