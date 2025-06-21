#pragma once
#include <Arduino.h>
#include <string>

#define LINE_BUFFER_LENGTH 512

class GcodeParserService
{
private:
  GcodeParserService();

public:
  static GcodeParserService &getInstance();

  void setup();
  bool parseLine(const std::string &line);
};
