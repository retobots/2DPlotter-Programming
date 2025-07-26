#include "SDCardHAL.h"
using namespace std;

SDCardHAL::SDCardHAL()
{
  // Constructor logic if needed
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

void SDCardHAL::loadFileListFromSD()
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

void SDCardHAL::readSelectedFile(String &filename)
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