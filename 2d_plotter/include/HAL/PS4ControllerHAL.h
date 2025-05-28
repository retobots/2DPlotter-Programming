#pragma once
#include <Arduino.h>
class PS4ControllerHAL {
public:
  static PS4ControllerHAL& getInstance();

  void setup();
  int getAnalogX();
  int getAnalogY();
  bool isButtonPressed();

private:
  PS4ControllerHAL();
};
