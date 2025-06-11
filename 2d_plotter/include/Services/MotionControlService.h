#pragma once
#include <Arduino.h>
class MotionControlService {
public:
  static MotionControlService& getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();

private:
  MotionControlService();
};
