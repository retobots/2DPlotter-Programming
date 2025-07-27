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
  vector<String> fileList;

  // Vector to store file contents
  vector<String> fileContents;

  // Constructor
  IoHwAb_SD();

  // Destructor
  ~IoHwAb_SD();

public:
  static IoHwAb_SD &getInstance();

  void setup();

  void loadFileListFromSD();

  void readSelectedFile(String &filename);

  File &getFile(const String &filename);
};
