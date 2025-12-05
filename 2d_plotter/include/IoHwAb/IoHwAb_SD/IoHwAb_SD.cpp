/*************************************************************************************************************************
 * @file    IoHwAb_SD.cpp
 * @brief   Implementation of functions to handle SD card operations
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include "IoHwAb_SD.h"
using namespace std;

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_SD::IoHwAb_SD()
{
  // Constructor logic if needed
}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_SD &IoHwAb_SD::getInstance()
{
  static IoHwAb_SD instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the SD card
 ************************************************************************************************************************/
void IoHwAb_SD::setup()
{
  SD.begin(PIN_SD_CS);

  Serial.println("[SET UP]: SD Card Done!");
}

/*************************************************************************************************************************
 * @brief Load the list of files from the SD card
 *
 * @return Reference to the vector containing the list of file names
 ************************************************************************************************************************/
vector<String> &IoHwAb_SD::loadFileListFromSD()
{
  fileList.clear();
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
  return fileList;
}

/*************************************************************************************************************************
 * @brief Read the selected file from the SD card and store its contents
 *
 * @param filename Name of the file to read
 ************************************************************************************************************************/
void IoHwAb_SD::readSelectedFile(String &filename)
{
  Serial.println("Reading file: " + filename);
  fileContents.clear();
  File f = SD.open(filename.c_str());
  if (!f)
  {
    Serial.println("Cannot open file!");
    return;
  }

  Serial.println("File opened successfully.");
  String line;
  size_t lineCount = 0;
  while (f.available())
  {
    char c = f.read();
    if (c == '\n' || c == '\r')
    {
      // if (!line.isEmpty())
      // {
      //   // fileContents.push_back(line);
      //   line.clear();
      // }
      if (!line.isEmpty())
      {
        fileContents.push_back(line);
        line.clear();
        lineCount++;

        if (lineCount % 50 == 0)
        {
          Serial.print("Lines: ");
          Serial.print(lineCount);
          Serial.print(" | Free heap: ");
          Serial.println(ESP.getFreeHeap());
        }
      }
    }
    else
    {
      line += c;
    }
  }

  Serial.println("File stored successfully.");

  if (!line.isEmpty())
  {
    // fileContents.push_back(line);
  }
  f.close();

  Serial.println("File read complete. Total lines: " + String(fileContents.size()));
}

/*************************************************************************************************************************
 * @brief Get a File object for the specified filename
 *
 * @param filename Name of the file to open
 * @return Reference to the File object
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief Get a character from the specified line and character index in the file contents
 *
 * @param lineIndex Index of the line in the file contents
 * @param charIndex Index of the character in the specified line
 * @return The character at the specified position, or '\0' if out of bounds
 ************************************************************************************************************************/
char IoHwAb_SD::getCharFromVectorLine(int lineIndex, int charIndex)
{
  if (lineIndex >= 0 && lineIndex < fileContents.size())
  {
    const String &line = fileContents[lineIndex];

    // Print the line only once when starting a new line
    if (charIndex == 0)
    {
      Serial.println("Get Line: " + line);
    }

    if (charIndex >= 0 && charIndex < line.length())
    {
      return line[charIndex];
    }
    if (charIndex == line.length())
    {
      return '\n';
    }
  }
  return '\0';
}

/*************************************************************************************************************************
 * @brief Check if the file contents vector is empty
 *
 * @return true if the file contents vector is empty, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_SD::isFileContentsEmpty() const
{
  return fileContents.empty();
}