#pragma once
#include <string>
#include <Arduino.h>
class LcdHAL {
public:
  static LcdHAL& getInstance();

  void setup();
  void print(const std::string& message);
  void clear();

private:
  LcdHAL();
};
