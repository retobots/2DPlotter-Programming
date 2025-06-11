#pragma once
#include <vector>
#include <string>
#include <Arduino.h>
class SDCardHAL {
public:
  static SDCardHAL& getInstance();

  void setup();
  std::vector<std::string> listFiles();
  std::string readLine();

  bool openFile(const std::string& filename);
  void closeFile();

private:
  SDCardHAL();
};
