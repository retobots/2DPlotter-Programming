#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "Settings.h"

class IoHwAb_Servo
{
private:
  IoHwAb_Servo();
  int servoChannel = 1; // PWM channel 1 for ESP32 (avoid conflict with buzzer channel 0)
  void setServoAngle(int angle);

public:
  static IoHwAb_Servo &getInstance();

  void setup();
  void liftPen();
  void dropPen();
};
