#pragma once
#include <Arduino.h>
#include "PS4Controller.h"

typedef struct
{
  float x;
  float y;
} PointData;

class PS4ControllerHAL
{
public:
  static PS4ControllerHAL &getInstance();

  void setup();
  PointData readPS4();
  bool isButtonPressed();

private:
  PS4ControllerHAL();
};
