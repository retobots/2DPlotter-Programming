#pragma once
#include <vector>
#include <string>
#include <Arduino.h>
#include <SD.h>

class SDCardHAL
{
private:
  SDCardHAL();
  // String fileList[MAX_FILES];
  // int fileCount = 0;

public:
  static SDCardHAL &getInstance();

  void setup();
  std::vector<std::string> listFiles();
  std::string readLine();

  bool openFile(const std::string &filename);
  void closeFile();

  void loadFileListFromSD();

  void readSelectedFile(String filename);
};
