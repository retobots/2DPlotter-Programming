#pragma once
#include <Arduino.h>
class ServoHAL {
public:
  static ServoHAL& getInstance();

  void setup();
  void liftPen();
  void dropPen();

private:
  ServoHAL();
};
