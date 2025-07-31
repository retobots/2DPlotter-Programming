#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "Settings.h"

class IoHwAb_Servo
{
private:
  IoHwAb_Servo();

  void setServoAngle(int angle);

public:
  static IoHwAb_Servo &getInstance();

  void setup();
  void liftPen();
  void dropPen();
};
