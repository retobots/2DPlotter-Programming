#pragma once
#include <Arduino.h>
class AutoModeController {
public:
  static AutoModeController& getInstance();

  void setup();
  void run();

private:
  AutoModeController();
};
