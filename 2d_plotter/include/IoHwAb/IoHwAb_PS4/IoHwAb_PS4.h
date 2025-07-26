#pragma once
#include <Arduino.h>
#include "PS4Controller/PS4Controller.h"
#include "Settings.h"

class IoHwAb_PS4
{
public:
  static IoHwAb_PS4 &getInstance();

  void setup();
  point readPS4();
  bool isButtonPressed();

private:
  IoHwAb_PS4();
};
