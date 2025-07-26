#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"
#include "Settings.h"

class ServoHAL
{
private:
  ServoHAL();
  Servo myServo;

public:
  static ServoHAL &getInstance();

  void setup();
  void liftPen();
  void dropPen();
};
