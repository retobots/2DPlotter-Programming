/*************************************************************************************************************************
 * @file    IoHwAb_SD.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#pragma once

#include <vector>
#include <string>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <map>
#include "Pins.h"
using namespace std;

class IoHwAb_SD
{
private:
  // Vector to store file names
  vector<String> fileList = vector<String>(); // empty for now

  // Vector to store file contents
  vector<String> fileContents = vector<String>(); // empty for now

  // Constructor
  IoHwAb_SD();

  // Destructor
  ~IoHwAb_SD() = default;

public:
  static IoHwAb_SD &getInstance();

  void setup();

  vector<String> &loadFileListFromSD();

  void readSelectedFile(String &filename);

  File &getFile(const String &filename);

  char getCharFromVectorLine(int lineIndex, int charIndex);

  bool isFileContentsEmpty() const;
};
