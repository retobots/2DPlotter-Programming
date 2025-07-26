#pragma once
#include <vector>
#include <string>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <map>
#include "Pins.h"
using namespace std;

class SDCardHAL
{
private:
  // Vector to store file names
  vector<String> fileList;

  // Vector to store file contents
  vector<String> fileContents;

  // Constructor
  SDCardHAL();

  // Destructor
  ~SDCardHAL();

public:
  static SDCardHAL &getInstance();

  void setup();

  void loadFileListFromSD();

  void readSelectedFile(String &filename);
};
