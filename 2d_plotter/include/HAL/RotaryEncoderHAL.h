#pragma once
#include <Arduino.h>
class RotaryEncoderHAL {
public:
  static RotaryEncoderHAL& getInstance();

  void setup();
  int readRotation();  // +1, -1, hoặc 0
  bool isButtonPressed();

private:
  RotaryEncoderHAL();
};
