#pragma once
#include <Arduino.h>
#include <string>

class GcodeParserService {
public:
  static GcodeParserService& getInstance();

  void setup();
  bool parseLine(const std::string& line);

private:
  GcodeParserService();
};
