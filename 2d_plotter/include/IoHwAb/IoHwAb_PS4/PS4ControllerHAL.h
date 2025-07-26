#pragma once
#include <Arduino.h>
#include "PS4Controller/PS4Controller.h"
#include "Settings.h"

class PS4ControllerHAL
{
public:
  static PS4ControllerHAL &getInstance();

  void setup();
  point readPS4();
  bool isButtonPressed();

private:
  PS4ControllerHAL();
};
