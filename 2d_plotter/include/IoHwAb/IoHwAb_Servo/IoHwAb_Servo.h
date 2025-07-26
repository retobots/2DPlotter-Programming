#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"
#include "Settings.h"

class IoHwAb_Servo
{
private:
  IoHwAb_Servo();
  Servo myServo;

public:
  static IoHwAb_Servo &getInstance();

  void setup();
  void liftPen();
  void dropPen();
};
