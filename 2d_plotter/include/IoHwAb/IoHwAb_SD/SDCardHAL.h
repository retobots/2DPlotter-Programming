#pragma once
#include <vector>
#include <string>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <map>
#include "Pins.h"

class SDCardHAL
{
private:
  SDCardHAL();

  // Vector to store file names
  vector<string> fileList;

  // Vector to store file contents
  vector<string> fileContents;

  // Constructor
  SDCardHAL() = default;
  // Destructor
  ~SDCardHAL() = default;

public:
  static SDCardHAL &getInstance();

  void setup();

  string readLine();

  void loadFileListFromSD();

  void readSelectedFile(String filename);
};
